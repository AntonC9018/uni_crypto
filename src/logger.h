struct Logger
{
    bool has_errors;
};

inline Logger make_logger()
{
    return {0};
}

#define logger_add_error(logger, string, ...) { (logger).has_errors = true; fprintf((stderr), (string), ## __VA_ARGS__); }