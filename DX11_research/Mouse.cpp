#pragma once

#include "Mouse.h"
#include "Windows.h"

std::pair<int, int> Mouse::get_pos() const noexcept
{
	return { x_, y_ };
}

int Mouse::get_pos_x() const noexcept
{
	return x_;
}

int Mouse::get_pos_y() const noexcept
{
	return y_;
}

bool Mouse::left_is_pressed() const noexcept
{
	return mouse_left_is_pressed_;
}

bool Mouse::right_is_pressed() const noexcept
{
	return mouse_right_is_pressed_;
}

Mouse::Event Mouse::read() noexcept
{
	if (buffer_.size() > 0u)
	{
		Mouse::Event event = buffer_.front();
		buffer_.pop();
		return event;
	}

	return Mouse::Event();
}

void Mouse::flush() noexcept
{
	buffer_ = std::queue<Event>();
}

void Mouse::on_mouse_move(int in_x, int in_y) noexcept
{
	x_ = in_x;
	y_ = in_y;

	buffer_.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	trim_buffer();
}

void Mouse::on_left_mb_pressed(int x, int y) noexcept
{
	mouse_left_is_pressed_ = true;

	buffer_.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	trim_buffer();
}

void Mouse::on_right_mb_pressed(int x, int y) noexcept
{
	mouse_right_is_pressed_ = true;

	buffer_.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	trim_buffer();
}

void Mouse::on_left_mb_released(int x, int y) noexcept
{
	mouse_left_is_pressed_ = false;

	buffer_.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	trim_buffer();
}

void Mouse::on_right_mb_released(int x, int y) noexcept
{
	mouse_left_is_pressed_ = false;

	buffer_.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	trim_buffer();
}

void Mouse::on_wheel_up(int x, int y) noexcept
{
	buffer_.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	trim_buffer();
}

void Mouse::on_wheel_down(int x, int y) noexcept
{
	buffer_.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	trim_buffer();
}

void Mouse::trim_buffer() noexcept
{
	while (buffer_.size() > buffer_size_)
	{
		buffer_.pop();
	}
}

bool Mouse::is_in_window() const noexcept
{
	return is_in_window_;
}

void Mouse::on_mouse_enter() noexcept
{
	is_in_window_ = true;
	buffer_.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	trim_buffer();
}

void Mouse::on_mouse_leave() noexcept
{
	is_in_window_ = true;
	buffer_.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	trim_buffer();
}

void Mouse::on_wheel_delta(int x, int y, int delta)
{
	wheel_delta_carry_ += delta;

	while (wheel_delta_carry_ >= WHEEL_DELTA )
	{
		wheel_delta_carry_ -= WHEEL_DELTA;
		on_wheel_up(x, y);
	}
	while (wheel_delta_carry_ <= -WHEEL_DELTA )
	{
		wheel_delta_carry_ += WHEEL_DELTA;
		on_wheel_down(x, y);
	}
}
