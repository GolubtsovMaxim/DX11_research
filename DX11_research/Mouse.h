#pragma once

#include <queue>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};
	private:
		Type type_;
		bool event_left_is_pressed_;
		bool event_right_is_pressed_;
		int event_x_;
		int event_y_;
	public:
		Event() noexcept :
			type_(Type::Invalid),
			event_left_is_pressed_(false),
			event_right_is_pressed_(false),
			event_x_(0),
			event_y_(0)
		{
		}

		Event(Type in_type, const Mouse& in_parent) noexcept :
			type_(in_type),
			event_left_is_pressed_(in_parent.mouse_left_is_pressed_),
			event_right_is_pressed_(in_parent.mouse_right_is_pressed_),
			event_x_(in_parent.x_),
			event_y_(in_parent.y_)
		{

		}
		bool is_valid() const noexcept
		{
			return type_ != Type::Invalid;
		}
		Type get_type() const noexcept
		{
			return type_;
		}
		[[nodiscard]] std::pair<int, int> get_pos() const noexcept
		{
			return { event_x_,event_y_ };
		}
		[[nodiscard]] int get_pos_x() const noexcept
		{
			return event_x_;
		}
		[[nodiscard]] int get_pos_y() const noexcept
		{
			return event_y_;
		}
		[[nodiscard]] bool left_is_pressed() const noexcept
		{
			return event_left_is_pressed_;
		}
		[[nodiscard]] bool right_is_pressed() const noexcept
		{
			return event_right_is_pressed_;
		}
	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator= (const Mouse&) = delete;
	std::pair<int, int> get_pos() const noexcept;
	int get_pos_x() const noexcept;
	int get_pos_y() const noexcept;
	bool is_in_window() const noexcept;
	bool left_is_pressed() const noexcept;
	bool right_is_pressed() const noexcept;
	Mouse::Event read() noexcept;
	bool is_empty() const noexcept
	{
		return buffer_.empty();
	};
	void flush() noexcept;
private:
	void on_mouse_move(int in_x, int in_y) noexcept;
	void on_left_mb_pressed(int x, int y) noexcept;
	void on_left_mb_released(int x, int y) noexcept;
	void on_right_mb_pressed(int x, int y) noexcept;
	void on_right_mb_released(int x, int y) noexcept;
	void on_mouse_leave() noexcept;
	void on_mouse_enter() noexcept;
	void on_wheel_up(int x, int y) noexcept;
	void on_wheel_down(int x, int y) noexcept;

	void trim_buffer() noexcept;
	void on_wheel_delta(int x, int y, int delta);

private:
	static constexpr unsigned buffer_size_ = 16u;
	int x_;
	int y_;
	bool mouse_left_is_pressed_;
	bool mouse_right_is_pressed_;
	bool is_in_window_ = false;
	int wheel_delta_carry_ = 0;
	std::queue<Event> buffer_;
};