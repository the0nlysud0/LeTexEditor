#pragma once

#include <Windows.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <variant>
#ifdef D_DEBUG

template<typename... Args>
void text_log(Args... args) {
    (std::cout << ... << args);
}
#define LOG(...) text_log(__VA_ARGS__)
#else
#define LOG(...)
#endif // _DEBUG

// std::filesystem::path g_ExecutablePath;
// std::filesystem::path g_ShaderPath;
// std::filesystem::path g_FontPath;

enum paths{
  EXE_PATH,
  SHADER_PATH,
  FONT_PATH,
  PARAMS_PATH,
};

enum class FileType{
  Image,
  Binary,
  Text,
};

inline std::string OpenFileDialog(const char* filter = "All Files\0*.*\0") {
  char filename[MAX_PATH] = { 0 };

  OPENFILENAMEA ofn = { 0 };
  ofn.lStructSize = sizeof(ofn);
  ofn.lpstrFile = filename;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  ofn.lpstrTitle = "Select a File";

  if (GetOpenFileNameA(&ofn)) {
      return std::string(filename);
  }

  return "";
}

inline FileType GetFileType(const std::string& path)
{
    std::filesystem::path filePath(path);
    std::string ext = filePath.extension().string();

    if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp" || ext == ".gif") {
        return FileType::Image;
    } else if (ext == ".bin" || ext == ".dat"|| ext == ".exe") {
        return FileType::Binary;
    } else {
        return FileType::Text;
    }
}

inline std::string ReadTextFile(std::string FilePath)
{
  std::ifstream File(FilePath);
  if(!File.is_open())
  {
    LOG("ERROR::FILE::IO::failed to read file on path: " , FilePath , '\n');
    return "";
  }
  std::stringstream iss;
  iss << File.rdbuf();
  LOG("INFO::FILE::IO::successfully read file on path: " , FilePath , '\n');
  // LOG(iss.str(),'\n');
  return iss.str();
}

inline std::vector<char> ReadBinaryFile(const std::string& path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate); // open at end
  if (!file.is_open()) {
      throw std::runtime_error("INFO::FILE::IO::failed to open binary file: " + path);
  }

  std::streamsize size = file.tellg(); // get size
  file.seekg(0, std::ios::beg); // rewind

  std::vector<char> buffer(size);
  if (!file.read(buffer.data(), size)) {
      throw std::runtime_error("INFO::FILE::IO::failed to read binary file: " + path);
  }

  return buffer;
}

inline std::string ReadBinaryAsString(const std::string& path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
      LOG("ERROR::FILE::IO::Failed to open binary file: ", path, '\n');
      return "";
  }

  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::string buffer(size, '\0');
  if (!file.read(&buffer[0], size)) {
      LOG("ERROR::FILE::IO::Failed to read binary file: ", path, '\n');
      return "";
  }

  return buffer;
}


inline std::string ReadFile(const std::string& FilePath)
{
  switch (GetFileType(FilePath))
  {
  case FileType::Text:
    return ReadTextFile(FilePath);
    break;
  case FileType::Binary:
    return ReadBinaryAsString(FilePath);
  default:
    return "";
    break;
  }
}

inline void Write2File(std::string FilePath, std::string Text2Write) {
  std::ofstream WrFile(FilePath);
  if (!WrFile.is_open()) {
      LOG("ERROR::FILE::IO::failed to open file on path: ", FilePath, '\n');
      return;
  }
  WrFile << Text2Write;
  LOG("INFO::FILE::IO::successfully wrote to file on path: ", FilePath, '\n');
}

inline std::string GetExecutableDir(char* argv0)
{
  std::filesystem::path exePath = std::filesystem::absolute(argv0);
  std::filesystem::path exeDir = exePath.parent_path();
  return exeDir.string();
}

inline std::string getPath(char* argv0,paths param)
{
  switch (param)
  {
  case EXE_PATH:
    return std::filesystem::absolute(argv0).parent_path().lexically_normal().string();
    break;
  case SHADER_PATH:
    return (std::filesystem::absolute(argv0).parent_path() / "../../Assets/Shaders/").lexically_normal().string();
    break;
    case FONT_PATH:
    return (std::filesystem::absolute(argv0).parent_path() / "../../Assets/Fonts/").lexically_normal().string();
    break;
    case PARAMS_PATH:
    return (std::filesystem::absolute(argv0).parent_path() / "../../Assets/Settings/").lexically_normal().string();
  default:
    return "";
    break;
  }
}