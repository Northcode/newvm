
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <regex>

#include "constants.hpp"
#include "registers.hpp"

using namespace std;

enum argtype
  {
    argtype_reg,
    argtype_byte,
    argtype_dword,
    argtype_int
  };

struct instruction
{
  byte opcode;
  vector<argtype> argument_types;

  instruction(byte o,vector<argtype> a) : opcode(o) , argument_types(a) {}
  
};

const map<string,instruction> instruction_set = {
  { "exit", { 0, {} } },
  { "mv" ,  { 1, { argtype_reg, argtype_reg   }}},
  { "ldb",  { 2, { argtype_reg, argtype_byte  }}},
  { "lddw", { 3, { argtype_reg, argtype_dword }}},
  { "ldi",  { 4, { argtype_reg, argtype_int   }}},
  { "cl",   { 5, { argtype_reg }}}
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
  
};
