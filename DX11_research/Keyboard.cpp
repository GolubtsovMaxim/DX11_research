#include "Keyboard.h"

bool Keyboard::key_is_pressed(unsigned char in_keycode)
{
	return key_states[in_keycode];
}

Keyboard::Event Keyboard::read_key() noexcept
{
	if (key_buffer.size() > 0u)
	{
		Keyboard::Event event = key_buffer.front();
		key_buffer.pop();
		return event;
	}
	else
	{
		return Keyboard::Event();
	}
}

bool Keyboard::key_is_empty() const noexcept
{
	return key_buffer.empty();
}

char Keyboard::read_char() noexcept
{
	if (char_buffer.size() > 0u)
	{
		char code = char_buffer.front();
		char_buffer.pop();
		return code;
	}
	else
	{
		return 0;
	}
}

bool Keyboard::char_is_empty() const noexcept
{
	return char_buffer.empty();
}

void Keyboard::flush_key() noexcept
{
	key_buffer = std::queue<Event>();
}

void Keyboard::flush_char() noexcept
{
	char_buffer = std::queue<char>();
}

void Keyboard::flush() noexcept
{
	flush_key();
	flush_char();
}

void Keyboard::enable_autorepeat() noexcept
{
	auto_repeat_enabled = true;
}

void Keyboard::disable_autorepeat() noexcept
{
	auto_repeat_enabled = false;
}

bool Keyboard::autorepeat_is_enabled() noexcept
{
	return auto_repeat_enabled;
}

void Keyboard::on_key_pressed(unsigned char in_keycode) noexcept
{
	key_states[in_keycode] = true;
	key_buffer.push(Keyboard::Event (Keyboard::Event::Type::PRESSED, in_keycode));
	trim_buffer(key_buffer);
}

void Keyboard::on_key_released(unsigned char in_keycode) noexcept
{
	key_states[in_keycode] = false;
	key_buffer.push(Keyboard::Event(Keyboard::Event::Type::RELEASED, in_keycode));
	trim_buffer(key_buffer);
}

void Keyboard::on_char(char in_character) noexcept
{
	char_buffer.push(in_character);
	trim_buffer(char_buffer);
}

void Keyboard::clear_state() noexcept
{
	key_states.reset();
}

template <typename T>
void Keyboard::trim_buffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > buffer_size)
	{
		buffer.pop();
	}
}

