#include <string>

class Msg {
private:
	std::string msg;

public:
	Msg(std::string message) : msg(message) {}
	std::string getMessage() {
		return msg;
	}
};

