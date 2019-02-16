#include <string>
#include <vector>
#include <map>

#include <openvr.h>

// Some string based utilites
namespace sutil{
	// Converts (naively) to lower case. Should be good enough for our case
	std::string stringtolower(std::string s){
		std::string out = s;

		for (int i = 0; i < s.size(); i++)
			out[i] = tolower(s[i]);

		return out;
	}
	
	// Removes whitespace
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
		/*
		  oculus		veve
		  A / X			Menu
		  B / Y			System
		  Grip			Grip
		  Trigger		Trigger
		  Joystick		Touchpad
		*/

		// Oculus Bindings
		{ "a", vr::EVRButtonId::k_EButton_A },
		{ "x", vr::EVRButtonId::k_EButton_A },

		{ "b", vr::EVRButtonId::k_EButton_ApplicationMenu },
		{ "y", vr::EVRButtonId::k_EButton_ApplicationMenu },

		{ "joystick", vr::EVRButtonId::k_EButton_Axis0 },

		// Vive Bindings
		{"menu", vr::EVRButtonId::k_EButton_ApplicationMenu },
		{"system", vr::EVRButtonId::k_EButton_System },

		{"touchpad", vr::EVRButtonId::k_EButton_SteamVR_Touchpad },

		// Common Bindings
		{ "grip", vr::EVRButtonId::k_EButton_Grip },
		{ "trigger", vr::EVRButtonId::k_EButton_Axis1 }
	};

	// Bind struct, contains info about a binding.
	struct bind {
		uint64_t mask_left_or, mask_right_or;	// Masks for left and right OR registers
		uint64_t mask_left_and, mask_right_and; // Masks for left and right AND registers

		uint8_t active_registers;				// What registers currently have an effect

	public:
		bind() {
			this->active_registers = 0x0; // Default to all registers being off
		}

		bind(std::string value) {
			this->active_registers = 0x0; // Default to all registers being off

			this->mask_left_or = 0x0;
			this->mask_right_or = 0x0;
			this->mask_left_and = 0x0;
			this->mask_right_and = 0x0;

			if (value.size() == 0) return;

			std::map <std::string, std::vector <std::string>> args;

			std::string buf = "";
			std::string pre_controller = "";
			std::string controller = "both";
			std::string entry = "";

			uint8_t register_write_pre = 0xC; // Start writing to both or registers by default
			uint8_t register_write = 0xC; 

			uint8_t register_write_temp = 0x0;

			// Parse input character by character.
			for (int i = 0; i < value.size() + 1; i++) {
				char c = value[i];

				switch (c) {
				case ':':
					controller = sutil::stringtolower(sutil::trimspace(buf));

					// Break if this controller is unsupported
					if (!((controller == "left") || (controller == "right") || (controller == "both"))) throw std::exception(("Unknown controller alias '" + entry + "'").c_str());

					buf = "";
					break;

				case '|': case '&':
					// Arm selected register for write
					register_write_pre = (c == '|' ? 0xC : 0x3);

				case ',': case '\0': // New entry or EOF
					entry = sutil::stringtolower(sutil::trimspace(buf));

					if (!(entry.size() > 0)) break; // Skip if nothing found in buffer

					register_write_temp = register_write;
					register_write_temp &= ((controller == "left" ? 0xA : 0x5) | (controller == "both" ? 0xF : 0x0));

					// Break if this button is unrecognized
					if (controller_aliases.count(entry) == 0) throw std::exception(("Unkown button alias '" + entry + "'").c_str());

					if ((register_write_temp & 0x8)) this->mask_left_or   |= ButtonMaskFromID(controller_aliases[entry]);
					if ((register_write_temp & 0x4)) this->mask_right_or  |= ButtonMaskFromID(controller_aliases[entry]);
					if ((register_write_temp & 0x2)) this->mask_left_and  |= ButtonMaskFromID(controller_aliases[entry]);
					if ((register_write_temp & 0x1)) this->mask_right_and |= ButtonMaskFromID(controller_aliases[entry]);

					// Activate this register since we wrote to it
					this->active_registers |= register_write_temp;

					// Write pre into actual register
					register_write = register_write_pre;

					buf = "";
					break;

				default:
					buf += c;
					break;
				}
			}
		}
	};

	// Evaluates binding against current button states
	bool eval(bind b, uint64_t buttons_left, uint64_t buttons_right){
		uint64_t state = 0x0; // Default to off

		if (checkAllAny(buttons_left, b.mask_left_or)) state |= 0x8;
		if (checkAllAny(buttons_right, b.mask_right_or)) state |= 0x4;
		if (checkAll(buttons_left, b.mask_left_and)) state |= 0x2;
		if (checkAll(buttons_right, b.mask_right_and)) state |= 0x1;

		return (((b.active_registers & state) == b.active_registers? 0xFFFFFFF : 0x0) & b.active_registers);
	}
}