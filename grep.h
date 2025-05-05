class Grep{
  public:
  Grep(const char *pattern);
  bool scan(char);
  void set(const char *pattern);
  bool is(const char*);
  private:
  const char *pattern, *match;
  void shift2match(void);
  bool matchPattern(const char*);
};