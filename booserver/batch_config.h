#include <string>
#include <vector>

class BatchFile {
  std::string name;
  std::string title;
  
public:
  const std::string &getTitle(void) const { return title; }
  const std::string &getFilename(void) const { return name; }
  
  BatchFile(const std::string &name, const std::string &title);
};

class BatchConfig {
  std::vector<BatchFile> colors;
  std::vector<BatchFile> images;
  
  BatchConfig();
public:
  const std::vector<BatchFile> &getColors(void) const { return colors; }
  const std::vector<BatchFile> &getImages(void) const { return images; }
  
  static BatchConfig get(const std::string &path);
};

