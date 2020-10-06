namespace Kronos {
	_declspec(dllimport) void say_hello();
}

int main() {
	Kronos::say_hello();
	return 0;
}