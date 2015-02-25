
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>

#include "constants.hpp"
#include "registers.hpp"

using namespace std;

enum argtype
  {
    argtype_reg,
    argtype_byte,
    argtype_dword,
    argtype_int,
    argtype_cmp
  };

struct instruction
{
  byte opcode;
  vector<argtype> argument_types;

  instruction(byte o,vector<argtype> a) : opcode(o) , argument_types(a) {}
  
};

const map<string,instruction> instruction_set = {
  { "exit",  { 0,  {} } },
  { "mv" ,   { 1,  { argtype_reg, argtype_reg   }}},
  { "ldb",   { 2,  { argtype_reg, argtype_byte  }}},
  { "lddw",  { 3,  { argtype_reg, argtype_dword }}},
  { "ldi",   { 4,  { argtype_reg, argtype_int   }}},
  { "cl",    { 5,  { argtype_reg }}},
  { "jmpd",  { 6,  { argtype_dword }}},
  { "jmpr",  { 7,  { argtype_reg }}},
  { "calld", { 8,  { argtype_dword }}},
  { "callr", { 9,  { argtype_reg }}},
  { "int",   { 10, { argtype_dword }}},
  { "cjmpd", { 11, { argtype_cmp, argtype_dword }}},
  { "cjmpr", { 12, { argtype_cmp, argtype_reg }}},
  { "pushr", { 13, { argtype_reg }}},
  { "pushb", { 14, { argtype_byte }}},
  { "pushdw",{ 15, { argtype_dword }}},
  { "pushi", { 16, { argtype_int }}},
  { "popb",  { 17, { argtype_reg }}},
  { "popdw", { 18, { argtype_reg }}},
  { "popi",  { 19, { argtype_reg }}},
  { "outb",  { 20, { argtype_byte, argtype_byte }}},
  { "outbr", { 21, { argtype_byte, argtype_reg  }}},
  { "outdw", { 22, { argtype_byte, argtype_dword}}},
  { "outdwr",{ 23, { argtype_byte, argtype_reg  }}},
  { "outi",  { 24, { argtype_byte, argtype_int  }}},
  { "outir", { 25, { argtype_byte, argtype_reg  }}},
  { "inb",   { 26, { argtype_byte, argtype_reg  }}},
  { "indw",  { 27, { argtype_byte, argtype_reg  }}},
  { "ini",   { 28, { argtype_byte, argtype_reg  }}},
  { "add",   { 29, { argtype_reg, argtype_reg }}},
  { "sub",   { 30, { argtype_reg, argtype_reg }}},
  { "mul",   { 31, { argtype_reg, argtype_reg }}},
  { "div",   { 32, { argtype_reg, argtype_reg }}},
  { "mod",   { 33, { argtype_reg, argtype_reg }}},
  { "cmp",   { 34, { argtype_reg, argtype_reg }}},
  { "ret",   { 35, { }}}
};

const map<char,int> reg_map = {
  { 'B', 0 },
  { 'C', 1 },
  { 'D', 2 },
  { 'F', 3 }
};

// ldb A0 5
// split lines: (.*)(?:\n|$)
// remove comments: ([^\#]*)(?:\#.*)*
// matching regex: ([\w\d\:]+)

struct assembler
{
  vector<byte> program;
  
  string code;

  assembler(string code);

  void parse_all();

  void parse_line(string line);
  
  void writeToFile(string file);
};
