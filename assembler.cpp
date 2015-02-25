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

vector<byte> dword_to_bytes(dword value) {
  //cout << "decoding dword: " << value << endl;
  vector<byte> result = vector<byte>(4);
  result[3] = value >> 24 & 0x000000ff;
  result[2] = value >> 16 & 0x000000ff;
  result[1] = value >> 8  & 0x000000ff;
  result[0] = value       & 0x000000ff;
  return result;
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
  itr++;
  cout << ins << " : " << matchcount << " - " << instruction_set.count(ins) << endl;

  int ia = 0;

  if(instruction_set.count(ins) > 0) {
    auto instruction = instruction_set.at(ins);
    cout << (int)instruction.opcode << endl;
    if(matchcount - 1 != instruction.argument_types.size())
      return;
    program.push_back(instruction.opcode); ia++;
    cout << "args: ";
    for (auto arg : instruction.argument_types) {
      cout << (int)arg << ":";
      string match = itr->str(1);
      cout << match << " - ";
      if(arg == argtype_reg) {
	regex reg_regex{"([BCDF])(\\d+)"};
	cout << "matching regex: ";
	smatch m;
	if(!regex_match(match,m,reg_regex)) { cout << "error, " << match << " is not a register" << endl; return; }
	reg_selector reg{};
	reg.reg = reg_map.at(m[1].str()[0]);
	cout << " Regex match: " << m[2].str();
	reg.index = stoi(m[2].str());
	program.push_back(reg.raw); ia++;
      } else if (arg == argtype_byte) {
	byte b = (byte)stoi(match);
	program.push_back(b); ia++;
      } else if (arg == argtype_dword) {
	dword d = (dword)stoi(match);
	for (byte b : dword_to_bytes(d))
	  { program.push_back(b); ia++; }
      } else if (arg == argtype_int) {
	int i = stoi(match);
	for (byte b : dword_to_bytes(i))
	  { program.push_back(i); ia++; }
      } else if (arg == argtype_cmp) {
	regex cmpregex{"([EN])([TF])([GN])([TF])"};
	smatch m;
	if(!regex_match(match,m,cmpregex)) { cout << "error, " << match << " does not match comparison string" << endl; return; }
	cmp_byte cmp{};
	cout << "I HAS " << m.size() << " matches" << endl;
	cmp.eq  = (m[4].str().compare("T"));
	cmp.jeq = (m[3].str().compare("G"));
	cmp.gt  = (m[2].str().compare("T"));
	cmp.jgt = (m[1].str().compare("E"));
	cout << (cmp.jeq ? "E" : "N" ) << (cmp.eq ? "T" : "F") << (cmp.jgt ? "G" : "N" ) << (cmp.gt ? "T" : "F") << endl;
	program.push_back(cmp.raw); ia++;
      }
      cout << endl;
      
      itr++;
    }
  }

  while (ia < 8) {
    program.push_back(0);
    ia++;
  }

}

void assembler::writeToFile(string file) {
  ofstream filestream{file,ofstream::out};
  for (byte b : program)
    filestream << b;
  filestream.close();
}


/*

ldb B0 5
mv B0 B1
cl B0

*/


int main(int argc, char* argv[]) {
  string code{};
  if(argc > 1) {
    ifstream file{argv[1]};
    stringstream ss{};
    ss << file.rdbuf();
    code = ss.str();
  }
  assembler a{code};
  a.parse_all();
  for (byte b : a.program)
    cout << (int)b << ":";
  a.writeToFile("program.dat");
  return 0;
}
