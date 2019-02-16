#include <string>
#include <vector>
#include <map>

#include <bitset>
#include <iostream>

#include <openvr.h>

// Some string based utilites
namespace sutil {
	std::string stringtolower(std::string s){
		std::string out = s;

		for (int i = 0; i < s.size(); i++)
			out[i] = tolower(s[i]);

		return out;
	}

	std::string trimspace(std::string const& str){
		if (str.empty())
			return str;

		std::size_t firstScan = str.find_first_not_of(' ');
		std::size_t first = firstScan == std::string::npos ? str.length() : firstScan;
		std::size_t last = str.find_last_not_of(' ');
		return str.substr(first, last - first + 1);
	}
}

namespace bindings{
	inline uint64_t ButtonMaskFromID(vr::EVRButtonId id) { return 1ull << id; } // From openvr
	inline bool checkAll(uint64_t buttons, uint64_t mask) { return ((buttons & mask) == mask); } // Binding eval in AND mode
	inline bool checkAllAny(uint64_t buttons, uint64_t mask) { return (buttons & mask); } // Binding eval in OR mode

	// Mappings to convert from string aliases to respective EVRButtonIds
	std::map<std::string, vr::EVRButtonId> controller_aliases{
		{ "a", vr::EVRButtonId::k_EButton_A },
		{ "x", vr::EVRButtonId::k_EButton_A },

		{ "b", vr::EVRButtonId::k_EButton_ApplicationMenu },
		{ "y", vr::EVRButtonId::k_EButton_ApplicationMenu },

		{ "grip", vr::EVRButtonId::k_EButton_Grip },
		{ "joystick", vr::EVRButtonId::k_EButton_Axis0 }
	};

	enum activatemode {
		mode_any = 0,
		mode_all = 1,
		mode_any_controller_all = 2
	};

	struct bind {
		uint64_t mask_left;	 // Bitmask for the left controller 
		uint64_t mask_right; // Bitmask for the right controller
		activatemode mode; // Mode for binding (ALL || ANY)

	public:
		bind() {
			this->mode = activatemode::mode_any;
			this->mask_left = 0x0;
			this->mask_right = 0x0;
		}

		bind(std::string value) {
			this->mode = activatemode::mode_any;
			this->mask_left = 0;
			this->mask_right = 0;

			if (value.size() == 0) return;

			std::map <std::string, std::vector <std::string>> args;

			std::string buf = "";
			std::string controller = "both";

			this->mode = activatemode::mode_any;

			int _i = 0;
			if (value[0] == '&') { this->mode = activatemode::mode_all; _i = 1; }
			else if (value[0] == '|') { this->mode = activatemode::mode_any; _i = 1; }
			else if (value[0] == '~') { this->mode = activatemode::mode_any_controller_all; _i = 1; }

			for (int i = 0 + _i; i < value.size() + 1; i++) {
				char c = value[i];

				switch (c) {
				case ':':
					controller = sutil::stringtolower(sutil::trimspace(buf));
					buf = "";
					break;

				case ',': case '\0': // New entry or EOF
					args[controller].push_back(sutil::stringtolower(sutil::trimspace(buf)));
					buf = "";
					break;

				default:
					buf += c;
					break;
				}
			}

			if (args.count("left")) for (auto const& value : args["left"]){
				if (controller_aliases.count(value))
					this->mask_left |= ButtonMaskFromID(controller_aliases[value]);
				else throw std::exception(("Unkown button '" + value + "'").c_str());
			}

			if (args.count("right")) for (auto const& value : args["right"]){
				if (controller_aliases.count(value))
					this->mask_right |= ButtonMaskFromID(controller_aliases[value]);
				else throw std::exception(("Unkown button '" + value + "'").c_str());
			}

			for (auto const& value : args["both"]){
				if (controller_aliases.count(value)) {
					this->mask_left |= ButtonMaskFromID(controller_aliases[value]);
					this->mask_right |= ButtonMaskFromID(controller_aliases[value]);
				}
				else throw std::exception(("Unkown button '" + value + "'").c_str());
			}

			std::cout << "Evaluated L to: " << std::bitset<64>(this->mask_left) << std::endl;
			std::cout << "Evaluated R to: " << std::bitset<64>(this->mask_right) << std::endl;
		}
	};

	// Evaluates binding against current button states
	bool eval(bind b, uint64_t buttons_left, uint64_t buttons_right){
		switch (b.mode){
			case activatemode::mode_any: return (checkAllAny(buttons_left, b.mask_left) || checkAllAny(buttons_right, b.mask_right));

			case activatemode::mode_all: return ((checkAll(buttons_left, b.mask_left)) && (checkAll(buttons_right, b.mask_right)) & (b.mask_left | b.mask_right)); 

			case activatemode::mode_any_controller_all: return (checkAll(buttons_left, b.mask_left) || checkAll(buttons_right, b.mask_right));
		}

		return false;
	}
}