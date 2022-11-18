#ifndef FILE_PROCESSOR_H
#define FILE_PROCESSOR_H

#include <string>
#include <vector>
#include <fstream>

class ProgramOptions;

std::string processAllFiles(const std::vector<std::string>& fileNames,
                             const ProgramOptions& progOptions);


#endif // FILE_PROCESSOR_H
