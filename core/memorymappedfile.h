#ifndef __CLRS4_MEMORY_MAPPED_FILE__
#define __CLRS4_MEMORY_MAPPED_FILE__

#include <cassert>
#include <common.h>
#include <filesystem>
#include <iostream>
#include <stdexcept>

#if __linux__
#include <cerrno>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#elif _WIN32 || _WIN64
#include <windows.h>
#else
static_assert(false, "Unsupported OS\n");
#endif

namespace frozenca {

using namespace std;

class MemoryMappedFile {
public:
  static inline constexpr size_t new_file_size_ = (1UL << 20UL);
#if __linux__
  using handle_type = int;
#elif _WIN32 || _WIN64
  using handle_type = HANDLE;
#else
  static_assert(false, "Unsupported OS\n");
#endif
  using path_type = filesystem::path::value_type;

private:
  const filesystem::path path_;
  void *data_ = nullptr;
  size_t size_ = 0;

  handle_type handle_ = 0;
  int flags_ = 0;
#if _WIN32 || _WIN64
  handle_type mapped_handle_ = 0;
#endif // Windows

public:
  MemoryMappedFile(const filesystem::path &path) : path_{path} {
    bool exists = filesystem::exists(path);
    open_file(path.c_str(), exists);
    map_file();
  }

  ~MemoryMappedFile() noexcept {
    if (!data_) {
      return;
    }
    bool error = false;
    error = !unmap_file() || error;
    error = !close_file() || error;
  }

private:
  void open_file(const path_type *path, bool exists) {
#if __linux__
    flags_ = O_RDWR;
    if (!exists) {
      flags_ |= (O_CREAT | O_TRUNC);
    }
#ifdef _LARGEFILE64_SOURCE
    flags |= O_LARGEFILE;
#endif
    errno = 0;
    handle_ = open(path, flags_, S_IRWXU);
    if (errno != 0) {
      throw runtime_error("file open failed\n");
    }

    if (!exists) {
      if (ftruncate(handle_, new_file_size_) == -1) {
        throw runtime_error("failed setting file size\n");
      }
    }

    struct stat info;
    bool success = (fstat(handle_, &info) != -1);
    size_ = info.st_size;
    if (!success) {
      throw runtime_error("failed querying file size\n");
    }
#elif _WIN32 || _WIN64
    DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
    DWORD dwCreationDisposition = exists ? OPEN_EXISTING : CREATE_ALWAYS;
    DWORD dwFlagsandAttributes = FILE_ATTRIBUTE_TEMPORARY;
    handle_ = CreateFileW(path, dwDesiredAccess, FILE_SHARE_READ, 0,
                          dwCreationDisposition, dwFlagsandAttributes, 0);
    if (handle_ == INVALID_HANDLE_VALUE) {
      throw runtime_error("file open failed\n");
    }

    if (!exists) {
      LONG sizehigh = (new_file_size_ >> (sizeof(LONG) * 8));
      LONG sizelow = (new_file_size_ & 0xffffffff);
      DWORD result = SetFilePointer(handle_, sizelow, &sizehigh, FILE_BEGIN);
      if ((result == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR) ||
          !SetEndOfFile(handle_)) {
        throw runtime_error("failed setting file size\n");
      }
    }

    typedef BOOL(WINAPI * func)(HANDLE, PLARGE_INTEGER);
    HMODULE hmod = GetModuleHandleA("kernel32.dll");
    func get_size =
        reinterpret_cast<func>(GetProcAddress(hmod, "GetFileSizeEx"));
    if (get_size) {
      LARGE_INTEGER info;
      if (get_size(handle_, &info)) {
        int64_t size =
            ((static_cast<int64_t>(info.HighPart) << 32) | info.LowPart);
        size_ = static_cast<size_t>(size);
      } else {
        throw runtime_error("failed querying file size");
      }
    } else {
      DWORD hi = 0;
      DWORD low = 0;
      if ((low = GetFileSize(handle_, &hi)) != INVALID_FILE_SIZE) {
        int64_t size = (static_cast<int64_t>(hi) << 32) | low;
        size_ = static_cast<size_t>(size);
      } else {
        throw runtime_error("failed querying file size");
        return;
      }
    }
#else
    static_assert(false, "Unsupported OS\n");
#endif
  }

  void map_file() {
#if __linux__
    void *data =
        mmap(0, file_size_, PROT_READ | PROT_WRITE, MAP_SHARED, handle_, 0);
    if (data == reinterpret_cast<void *>(-1)) {
      throw runtime_error("failed mapping file");
    }
    data_ = data;
#elif _WIN32 || _WIN64
    DWORD protect = PAGE_READWRITE;
    mapped_handle_ = CreateFileMappingA(handle_, 0, protect, 0, 0, 0);
    if (!mapped_handle_) {
      throw runtime_error("failed mapping file");
    }

    DWORD access = FILE_MAP_WRITE;
    void *data = MapViewOfFileEx(mapped_handle_, access, 0, 0, size_, 0);
    if (!data) {
      throw runtime_error("failed mapping file");
    }
    data_ = data;
#else
    static_assert(false, "Unsupported OS\n");
#endif
  }

  bool close_file() noexcept {
#if __linux__
    return close(handle_) == 0;
#elif _WIN32 || _WIN64
    return CloseHandle(handle_);
#else
    static_assert(false, "Unsupported OS\n");
#endif
  }

  bool unmap_file() noexcept {
#if __linux__
    return (munmap(data_, file_size_) == 0);
#elif _WIN32 || _WIN64
    bool error = false;
    error = !UnmapViewOfFile(data_) || error;
    error = !CloseHandle(mapped_handle_) || error;
    mapped_handle_ = NULL;
    return !error;
#else
    static_assert(false, "Unsupported OS\n");
#endif
  }

public:
  void resize(ptrdiff_t new_size) {
    if (new_size < 0) {
      throw invalid_argument("new size < 0");
    }
    if (!data_) {
      throw runtime_error("file is closed\n");
    }
    if (!unmap_file()) {
      throw runtime_error("failed unmappping file\n");
    }
#if __linux__
    if (ftruncate(handle_, new_size) == -1) {
      throw runtime_error("failed resizing mapped file\n");
    }
#elif _WIN32 || _WIN64
    int64_t offset = SetFilePointer(handle_, 0, 0, FILE_CURRENT);
    if (offset == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR) {
      throw runtime_error("failed querying file pointer");
    }
    LONG sizehigh = (new_size >> (sizeof(LONG) * 8));
    LONG sizelow = (new_size & 0xffffffff);
    DWORD result = SetFilePointer(handle_, sizelow, &sizehigh, FILE_BEGIN);
    if ((result == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR) ||
        !SetEndOfFile(handle_)) {
      throw runtime_error("failed resizing mapped file");
    }
    sizehigh = (offset >> (sizeof(LONG) * 8));
    sizelow = (offset & 0xffffffff);
    SetFilePointer(handle_, sizelow, &sizehigh, FILE_BEGIN);
#else
    static_assert(false, "Unsupported OS\n");
#endif
    size_ = static_cast<size_t>(new_size);
    map_file();
  }

  [[nodiscard]] size_t size() const noexcept { return size_; }

  [[nodiscard]] void *data() noexcept { return data_; }

  [[nodiscard]] const void *data() const noexcept { return data_; }

  friend bool operator==(const MemoryMappedFile &mmfile1,
                         const MemoryMappedFile &mmfile2) {
    auto res =
        (mmfile1.path_ == mmfile2.path_ && mmfile1.data_ == mmfile2.data_ &&
         mmfile1.size_ == mmfile2.size_ && mmfile1.handle_ == mmfile2.handle_ &&
         mmfile1.flags_ == mmfile2.flags_);
#if _WIN32 || _WIN64
    res = res && (mmfile1.mapped_handle_ == mmfile2.mapped_handle_);
#endif // Windows
    return res;
  }

  friend bool operator!=(const MemoryMappedFile &mmfile1,
                         const MemoryMappedFile &mmfile2) {
    return !(mmfile1 == mmfile2);
  }
};

} // namespace frozenca

#endif //__CLRS4_MEMORY_MAPPED_FILE__
