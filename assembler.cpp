#include "assembler.hpp"

assembler::assembler(string code) : code(code) {
  program = vector<byte>();
  lbls = map<string,int>();
  jmps = vector<tuple<int,string>> ();
}

void assembler::parse_all() {
  
  //iterate through every line
  regex splitregex{"(.*)(?:\n|$)"};
  auto iterator = sregex_iterator(code.begin(),code.end(),splitregex);
  
  cout << "first pass..." << endl;
  // first pass: parse each line
  for (auto i = iterator; i != sregex_iterator{}; i++) {
    auto line = i->str(1);
    if (line == "") continue;
    parse_line(line);
  }
  
  cout << "second pass..." << endl;
  // --- second pass: fix labels

  // iterate through all label references
  for(auto jmp : jmps) {

    string referenced = get<1>(jmp);
    dword refip = get<0>(jmp);
    
    // check that the label exists
    if(lbls.count(referenced) > 0) {

      // get absolute address we are referencing
      dword address = lbls.at(referenced);
      dword ipaddress = address - address % 8; // get the address of the instruction (nearest lower multiple of 8)

      // check what addressing mode we are using
      addressing_mode adr_mode;
      adr_mode.raw = program.at(refip - 1);

      // if addressing_mode is IP relative
      if(adr_mode.no_reg == false && adr_mode.Dindex == REG_IP) {
	//lblip - refip
	if (address > refip) { //address is ahead of ip
	  adr_mode.negate = false;
	  address = address - refip;
	} else { //address is behind
	  adr_mode.negate = true;
	  address = refip - address;
	}
	program[refip - 1] = adr_mode.raw;
      }
      
      int i = 0;

      for(auto b : dword_to_bytes(address)) { 
	program[refip + i] = b; 
	i++; 
      }

    } else
      cout << "Call to label: " << referenced  << ", is an undefined reference" << endl;
  }

}

vector<string> take(vector<string> vec,int amount) {
  return vector<string>(vec.begin() + amount, vec.end());
}

void assembler::parse_line(string line) {
  //remove comments
  auto found = line.find("#");
  if(found != string::npos)
    line = line.substr(0,found);

  //parse instruction
  regex insregex{"([\\w\\d\\:\\+\\-]+)"};
  sregex_iterator itr{line.begin(),line.end(),insregex};
  
  ptrdiff_t matchcount{distance(sregex_iterator{line.begin(),line.end(),insregex},sregex_iterator{})};

  string ins = itr->str(1);
  itr++;
  //cout << ins << " : " << matchcount << " - " << instruction_set.count(ins) << endl;

  regex label_regex{"([a-zA-Z]+)\\:"};
  smatch label;
  if(regex_match(ins,label,label_regex)) {
    //cout << label[1].str() << " : " << program.size() << endl;
    lbls[label[1].str()] = program.size();
  }

  int ia = 0;

  if(instruction_set.count(ins) > 0) {
    auto instruction = instruction_set.at(ins);
    //cout << (int)instruction.opcode << endl;
    if(matchcount - 1 != instruction.argument_types.size())
      return;
    program.push_back(instruction.opcode); ia++;
    //cout << "args: ";
    for (auto arg : instruction.argument_types) {
      //cout << (int)arg << ":";
      string match = itr->str(1);
      //cout << match << " - ";
      if(arg == argtype_reg) {
	regex reg_regex{"([BCDF])(\\d+)"};
	//cout << "matching regex: ";
	smatch m;
	if(!regex_match(match,m,reg_regex)) { cout << "error, " << match << " is not a register" << endl; return; }
	reg_selector reg{};
	reg.reg = reg_map.at(m[1].str()[0]);
	//cout << " Regex match: " << m[2].str();
	reg.index = stoi(m[2].str());
	program.push_back(reg.raw); ia++;
      } else if (arg == argtype_byte) {
	byte b = (byte)stoi(match);
	program.push_back(b); ia++;
      } else if (arg == argtype_dword) {
	regex str_regex{"([a-zA-Z]+)"};
	if(regex_match(match,str_regex)) // check for label (is this a string?)
	  jmps.push_back(tuple<int,string>{ia,match});
	else {  
	  
	  dword d = (dword)stoi(match);
	  for (byte b : dword_to_bytes(d))
	    { program.push_back(b); ia++; }
	  
	}
      } else if (arg == argtype_int) {
	int i = stoi(match);
	for (byte b : dword_to_bytes(i))
	  { program.push_back(i); ia++; }
      } else if (arg == argtype_cmp) {
	regex cmpregex{"([EN])([TF])([GN])([TF])"};
	smatch m;
	if(!regex_match(match,m,cmpregex)) { cout << "error, " << match << " does not match comparison string" << endl; return; }
	cmp_byte cmp{};
	//cout << "I HAS " << m.size() << " matches" << endl;
	cmp.eq  = (m[4].str().compare("T"));
	cmp.jeq = (m[3].str().compare("G"));
	cmp.gt  = (m[2].str().compare("T"));
	cmp.jgt = (m[1].str().compare("E"));
	//cout << (cmp.jeq ? "E" : "N" ) << (cmp.eq ? "T" : "F") << (cmp.jgt ? "G" : "N" ) << (cmp.gt ? "T" : "F") << endl;
	program.push_back(cmp.raw); ia++;
      } else if (arg == argtype_adrmd) {
	regex adrmd_regex{"(\\w\\w)([\\+\\-])"};
	smatch m;
	if(!regex_match(match,m,adrmd_regex)) { cout << "not an addressing mode" << endl; return; }
	
	addressing_mode adrmd;
	adrmd.raw = 0;
	
	string offset_register{m[1].str()};
	if      (!offset_register.compare("SP"))
	  adrmd.Dindex = REG_SP;
	else if (!offset_register.compare("BP"))
	  adrmd.Dindex = REG_BP;
	else if (!offset_register.compare("IP"))
	  adrmd.Dindex = REG_IP;
	else if (!offset_register.compare("NL"))
	  adrmd.no_reg = true;
	else
	  { cout << "could not match relative address: " << offset_register << endl; return; }

	adrmd.negate = m[2].str().compare("-") == 0;
	program.push_back(adrmd.raw); ia++;
      }
      
      itr++;
    }
  } else if (!ins.compare("db")) {
    
    line = string(line.begin() + 3, line.end());
    vector<string> params;
    vector<char>   stringbuilder;

    bool escaped = false;
    bool quotes = false;
    for (auto c : line) {
      if(!quotes  && c == ' ') { params.push_back({stringbuilder.begin(),stringbuilder.end()}); continue; }
      if(!escaped && c == '"') { params.push_back({stringbuilder.begin(),stringbuilder.end()}); stringbuilder.clear(); continue; }
      if(!escaped && c == '\\') escaped = true;
      stringbuilder.push_back(c);
      escaped = false;
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
