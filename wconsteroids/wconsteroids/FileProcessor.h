#ifndef FILE_PROCESSOR_H
#define FILE_PROCESSOR_H

#include <string>
#include <vector>

class ProgramOptions;
struct ExecutionCtrlValues;

std::string processAllFiles(const ExecutionCtrlValues& excutionControl);

#endif // FILE_PROCESSOR_H
