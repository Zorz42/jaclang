namespace preprocessor {
    inline std::vector<std::string> imports_to_dump, include_dirs;
    inline bool dump_imports;

    void main(std::list<char>::iterator start, std::list<char>::iterator end, std::string file);
}
