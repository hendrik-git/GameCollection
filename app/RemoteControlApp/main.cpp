#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Slider, Checkbox, Vertical, Renderer, Button, Input, Menu, Radiobox, Toggle
#include "ftxui/component/component_base.hpp"	   // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, operator|, Element, size, xflex, text, WIDTH, hbox, vbox, EQUAL, border, GREATER_THAN
#include <SFML/Network.hpp>
#include <functional>  // for function
#include <memory>	   // for shared_ptr, allocator, __shared_ptr_access
#include <string>	   // for string, basic_string
#include <vector>	   // for vector

using namespace ftxui;

// Display a component nicely with a title on the left.
Component Wrap(std::string name, Component component)
{
	return Renderer(component,
					[name, component]
					{
						return hbox({
								   text(name) | size(WIDTH, EQUAL, 12),
								   separator(),
								   component->Render() | xflex,
							   }) |
							   xflex;
					});
}


int main(int argc, const char* argv[])
{
	using namespace std::chrono_literals;

	sf::UdpSocket socket;

	auto screen = ScreenInteractive::FitComponent();


	// -- Input ------------------------------------------------------------------
	std::string port_str;
	std::string user_message;
	auto		input = Input(&port_str, "");
	input			  = Wrap("Port", input);

	std::function<void()> on_connect_clicked = [&socket, &port_str, &user_message]()
	{
		if(port_str.empty()) 
		{
			user_message = fmt::format("Please enter a port first");
			return;
		}

		try
		{
			auto port = static_cast<unsigned short>(std::stoi(port_str));

			// check if the number is out of port range
			if(port == 0 || port > 65'535)
			{
				return;
			}

			// bind the socket to a port
			if(socket.bind(port) == sf::Socket::Status::Error)
			{
				user_message = fmt::format("Failed to connect socket to port {}", port);
			}
			else
			{
				user_message = fmt::format("Connected to port {}", port);
			}
		}
		catch(const std::exception& e)
		{
			user_message = e.what();
		}
	};


	auto button_connect = Button("Connect", on_connect_clicked);
	button_connect		= Wrap("Connect by TCP", button_connect);


	// -- Button -----------------------------------------------------------------
	// std::string			  button_label = "Quit Application";
	std::function<void()> on_button_clicked_;
	auto				  button = Button("Quit Application", screen.ExitLoopClosure());
	button						 = Wrap("Button", button);

	// -- Layout -----------------------------------------------------------------
	auto layout = Container::Vertical({
		button_connect,
		input,
		button,
	});

	auto component = Renderer(layout,
							  [&]
							  {
								  return vbox({text("Remote Control App"),
											   separator(),
											   input->Render(),
											   separator(),
											   button_connect->Render(),
											   separator(),
											   button->Render(),
											   separator(),
											   text(user_message)}) |
										 xflex | size(WIDTH, GREATER_THAN, 40) | border;
							  });

	screen.Loop(component);

	return 0;
}