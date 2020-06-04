enum section {auto_, section_data, section_bss, section_text, section_functions};

struct instruction {
    std::string inst, arg1, arg2;
    int size;
    section sect;
};

namespace asm_ {
    void main();

    void append_instruction(const std::string &inst, const std::string &arg1 = "", const std::string &arg2 = "", int size = 0, section sect=auto_);
    std::string onStack(int offset);

    inline unsigned long currentScopeOnStack = 0;

    inline std::vector<variable> stack;

    inline int stackPointer = 0;
    inline int biggestStackPointer = 0;

    void incStackPointer(int value);
    void decStackPointer(int value);

    inline int8_t currentRegister = 0;
    inline std::vector<std::string> availableRegisters[4];

    void nextRegister();
    void prevRegister();

    void pushToStack(variable source);

    std::string availableRegister(int8_t size, int8_t offset = 0);

    inline std::vector<instruction> instructions;
}
