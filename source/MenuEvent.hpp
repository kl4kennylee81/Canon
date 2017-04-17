#ifndef MenuEvent_hpp
#define MenuEvent_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Event.hpp"

class MenuEvent : public Event {
public:
	enum class MenuEventType : int {
		PAUSEGAME
	};

	MenuEventType _menuEventType;

	bool init() {
		Event::init();
		_eventType = Event::EventType::MENU;
		return true;
	}

	static std::shared_ptr<MenuEvent> alloc() {
		std::shared_ptr<MenuEvent> result = std::make_shared<MenuEvent>();
		return (result->init() ? result : nullptr);
	}

	MenuEvent() : Event() {}
};

class PauseGame : public MenuEvent {
public:
	PauseGame() : MenuEvent() {}
	bool pause;

	bool init(bool p) {
		pause = p;
		MenuEvent::init();
		_menuEventType = MenuEventType::PAUSEGAME;
		return true;
	}

	static std::shared_ptr<PauseGame> alloc(bool p) {
		std::shared_ptr<PauseGame> result = std::make_shared<PauseGame>();
		return (result->init(p) ? result : nullptr);
	}
};


#endif
