//
//  TemplateEditorController.hpp
//  Canon
//
//  Created by Kenneth Lee on 4/4/17.
//  Copyright � 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TemplateEditorController_hpp
#define TemplateEditorController_hpp

#include <stdio.h>
#include "BaseController.hpp"
#include "MenuGraph.hpp"
#include "World.hpp"
#include "WaveData.hpp"

enum class TemplateEditorState : int {

};

class TemplateEditorController : public BaseController {
private:
	std::shared_ptr<World> _world;

	/** the main node for the level editor
	* connect all related nodes to this node not the scene */
	std::shared_ptr<cugl::Node> _levelEditNode;

	std::shared_ptr<WaveEntry> _currentEntry;

public:
	virtual void attach(Observer* obs);

	virtual void detach(Observer* obs);

	virtual void notify(Event* e);

	/**
	* Update the observer state based on an event from the subject
	*/
	virtual void eventUpdate(Event* e);

	virtual bool update(float timestep, std::shared_ptr<MenuGraph> menuGraph);

	void setTemplate(std::shared_ptr<WaveEntry> entry);

	virtual bool init(std::shared_ptr<cugl::Node> node, std::shared_ptr<World> world);

	static std::shared_ptr<TemplateEditorController> alloc(std::shared_ptr<cugl::Node> node, std::shared_ptr<World> world) {
		std::shared_ptr<TemplateEditorController> result = std::make_shared<TemplateEditorController>();
		return (result->init(node, world) ? result : nullptr);
	}

};

#endif /* TemplateEditorController_hpp */
