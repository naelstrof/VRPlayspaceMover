#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <iostream>

#include <openvr.h>

inline uint64_t ButtonMaskFromID(vr::EVRButtonId id) { return 1ull << id; }
inline bool checkAll(uint64_t buttons, uint64_t mask) { return ((buttons & mask) == mask); } // Binding eval in AND mode
inline bool checkAllAny(uint64_t buttons, uint64_t mask) { return (buttons & mask); } // Binding eval in OR mode

// Mappings to convert from string aliases to respective EVRButtonIds
std::map<std::string, vr::EVRButtonId> controller_aliases{

	{ "A", vr::EVRButtonId::k_EButton_A },
	{ "X", vr::EVRButtonId::k_EButton_A },

	{ "B", vr::EVRButtonId::k_EButton_ApplicationMenu },
	{ "Y", vr::EVRButtonId::k_EButton_ApplicationMenu },

	{ "Grip", vr::EVRButtonId::k_EButton_Grip },
	{ "Joystick", vr::EVRButtonId::k_EButton_Axis0 }

};

enum binding_mode {
	mode_any = 0,
	mode_all = 1,
	mode_any_controller_all = 2
};

struct binding {
	uint64_t mask_left;	 // Bitmask for the left controller 
	uint64_t mask_right; // Bitmask for the right controller
	binding_mode mode; // Mode for binding (ALL || ANY)

public:
	binding(std::string value) {
		if (value.size() == 0) return;

		std::map <std::string, std::vector <std::string>> args;

		std::string buf = "";
		std::string controller = "";

		this->mode = binding_mode::mode_any;

		int _i = 0;
		if (value[0] == '&') { this->mode = binding_mode::mode_all; _i=1; }
		else if (value[0] == '|') { this->mode = binding_mode::mode_any; _i=1; }
		else if (value[0] == '~') { this->mode = binding_mode::mode_any_controller_all; _i=1; }

		for (int i = 0 + _i; i < value.size() + 1; i++) {
			char c = value[i];

			switch (c) {
			case ':':
				controller = buf;
				buf = "";
				break;

			case ',': case '\0': // New entry or EOF
				args[controller].push_back(buf);
				buf = "";
				break;

			default:
				buf += c;
				break;
			}
		}

		this->mask_left = 0;
		this->mask_right = 0;

		if (args.count("Left")) for (auto const& value : args["Left"])
			if (controller_aliases.count(value))
				this->mask_left |= ButtonMaskFromID(controller_aliases[value]);
			else throw std::exception(("Unkown button '" + value + "'").c_str());

		if (args.count("Right")) for (auto const& value : args["Right"])
			if (controller_aliases.count(value))
				this->mask_right |= ButtonMaskFromID(controller_aliases[value]);
			else throw std::exception(("Unkown button '" + value + "'").c_str());
	}
};

bool eval_binding(binding b, uint64_t buttons_left, uint64_t buttons_right)
{
	if (b.mode == binding_mode::mode_any)
		return (checkAllAny(buttons_left, b.mask_left) || checkAllAny(buttons_right, b.mask_right));

	if (b.mode == binding_mode::mode_all)
		return ((checkAll(buttons_left, b.mask_left)) && (checkAll(buttons_right, b.mask_right)));

	if (b.mode == binding_mode::mode_any_controller_all)
		return (checkAll(buttons_left, b.mask_left) || checkAll(buttons_right, b.mask_right));

	return false;
}