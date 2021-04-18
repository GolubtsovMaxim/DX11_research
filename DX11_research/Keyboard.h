#pragma once
#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			PRESSED,
			RELEASED,
			INVALID
		};
	private:
		Type type_;
		unsigned char code_;
	public:
		Event()
			:
			type_(Type::INVALID),
			code_(0u)
		{}
		Event(Type in_type, unsigned char in_code)
		{}

		bool isPressed() const noexcept
		{
			return type_ == Type::PRESSED;
		}

		bool isReleased() const noexcept
		{
			return type_ == Type::RELEASED;
		}

		bool isValid() const noexcept
		{
			return type_ != Type::INVALID;
		}

		unsigned char get_code() const noexcept
		{
			return code_;
		}
	};

	public:
		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator= (const Keyboard&) = delete;

		bool key_is_pressed(unsigned char in_keycode);
		Event read_key() noexcept;
		bool key_is_empty() const noexcept;
		void flush_key() noexcept;

		char read_char() noexcept;
		bool char_is_empty() const noexcept;
		void flush_char() noexcept;
		void flush() noexcept;

		void enable_autorepeat() noexcept;
		void disable_autorepeat() noexcept;
		bool autorepeat_is_enabled() noexcept;
	private:
		void on_key_pressed(unsigned char in_keycode) noexcept;
		void on_key_released(unsigned char in_keycode) noexcept;
		void on_char(char in_character) noexcept; //typing text in text box
		void clear_state() noexcept;

		template<typename T>
		static void trim_buffer(std::queue<T>& buffer) noexcept;

	private:
		static constexpr unsigned n_keys = 256u;
		static constexpr unsigned buffer_size = 16u;
		bool auto_repeat_enabled = false;
		std::bitset<n_keys> key_states;
		std::queue<Event> key_buffer;
		std::queue<char> char_buffer;
};