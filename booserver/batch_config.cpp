#include "batch_config.h"
#include <dirent.h>
#include <sys/stat.h>

const std::string PREFIX_COLOR("color_");
const std::string SUFFIX_COLOR(".conf");
const std::string SUFFIX_IMAGE("_2160.png");

std::vector<std::string> readDir(const std::string &path) {
  std::vector<std::string> result;
  
  struct dirent *entry;
  struct stat attr;
  
  DIR *dir = opendir(path.c_str());
  if (dir) {
    while (entry = readdir(dir)) {
      if (stat(entry->d_name, &attr) == 0) {
        if (S_ISREG(attr.st_mode))
          result.push_back(std::string(entry->d_name));
      }
    }
    
    closedir(dir);
  }
  
  return result;
}

bool startsWith(const std::string &lhs, const std::string &rhs) {
  if (lhs.empty() != rhs.empty())
    return false;
  
  if (lhs.length() < rhs.length())
    return false;
  
  return lhs.compare(0, rhs.length(), rhs) == 0;
}

bool endsWith(const std::string &lhs, const std::string &rhs) {
  if (lhs.empty() != rhs.empty())
    return false;
  
  if (lhs.length() < rhs.length())
    return false;
  
  return lhs.compare(lhs.length() - rhs.length(), rhs.length(), rhs) == 0;
}

isColor(const std::string &fileName) {
  return startsWith(fileName, PREFIX_COLOR)
   && endsWith(fileName, SUFFIX_COLOR);
}

isImage(const std::string &fileName) {
  return endsWith(fileName, SUFFIX_IMAGE);
}
  
BatchFile getColorConfig(const std::string &fileName) {
  std::string title = fileName.substr(PREFIX_COLOR.length(),
    fileName.length() - SUFFIX_COLOR.length() - PREFIX_COLOR.length());
  return BatchFile(fileName, title);
}

BatchFile getImageConfig(const std::string &fileName) {
  std::string title = fileName.substr(0, fileName.length() - SUFFIX_IMAGE.length());
  return BatchFile(fileName, title);
}

BatchFile::BatchFile(const std::string &name, const std::string &title):
  name(name),
  title(title) {}

BatchConfig::BatchConfig(){
}

BatchConfig BatchConfig::get(const std::string &path) {
  BatchConfig result;
  
  std::vector<std::string> files = readDir(path);
  
  for (const std::string &file : files) {
    if (isColor(file))
      result.colors.emplace_back(getColorConfig(file));
    
    else if (isImage(file))
      result.images.emplace_back(getImageConfig(file));
  }
  
  return result;
}
