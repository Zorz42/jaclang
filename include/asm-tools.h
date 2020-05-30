namespace at {
    void append_instruction(const std::string &instruction, const std::string &arg1 = "", const std::string &arg2 = "");
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
}
