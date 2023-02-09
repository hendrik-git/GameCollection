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

	// bind the socket to a port
	if(socket.bind(54000) == sf::Socket::Status::Error)
	{
		std::cout << "Failed to connect socket to port 54000\n";
		std::this_thread::sleep_for(10s);
	}
	else
	{
		std::cout << "Connected to port 54000\n";
		std::this_thread::sleep_for(10s);
	}

	auto screen = ScreenInteractive::FitComponent();


	// -- Input ------------------------------------------------------------------
	std::string port;
	auto		input = Input(&port, "");
	input			  = Wrap("Port", input);

	std::function<void()> on_connect_clicked = []() { std::cout << "Hello World"; };
	auto				  button_connect	 = Button("Connect", on_connect_clicked);
	button_connect							 = Wrap("Connect by TCP", button_connect);


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
								  return vbox({
											 text("Remote Control App"),
											 separator(),
											 input->Render(),
											 separator(),
											 button_connect->Render(),
											 separator(),
											 button->Render(),
										 }) |
										 xflex | size(WIDTH, GREATER_THAN, 40) | border;
							  });

	screen.Loop(component);

	return 0;
}