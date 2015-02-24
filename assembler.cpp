#include "assembler.hpp"

assembler::assembler(string code) : code(code) {
  program = vector<byte>();
}

void assembler::parse_all() {
  
  regex splitregex{"(.*)(?:\n|$)"};
  auto iterator = sregex_iterator(code.begin(),code.end(),splitregex);
  for (auto i = iterator; i != sregex_iterator{}; i++) {
    auto line = i->str(1);
    if (line == "") continue;
    parse_line(line);
  }

}

void assembler::parse_line(string line) {
  //remove comments
  auto found = line.find("#");
  if(found != string::npos)
    line = line.substr(0,found);

  //parse instruction
  regex insregex{"([\\w\\d\\:]+)"};
  sregex_iterator itr{line.begin(),line.end(),insregex};
  
  ptrdiff_t matchcount{distance(sregex_iterator{line.begin(),line.end(),insregex},sregex_iterator{})};

  string ins = itr->str(1);
  cout << ins << endl;

  if(instruction_set.count(ins) > 0) {
    auto instruction = instruction_set.at(ins);
    cout << (int)instruction.opcode << endl;
    if(matchcount - 1 != instruction.argument_types.size())
      return;
    dword i = 2;
    for (auto arg : instruction.argument_types) {
      cout << (int)arg << endl;
      string match = itr->str(i);
      if(arg == argtype_reg) {
	regex reg_regex{"([BCDF])(\d+)"};
	if(!regex_match(match,reg_regex)) return;
	sregex_iterator itr{match.begin(),match.
      }
    }
  }
}

int main() {
  assembler a{"mv B0 B1\nldb B1 5\ncl B0"};
  a.parse_all();
  return 0;
}
