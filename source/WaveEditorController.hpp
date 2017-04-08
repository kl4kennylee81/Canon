//
//  TemplateEditorController.hpp
//  Canon
//
//  Created by Kenneth Lee on 4/4/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef WaveEditorController_hpp
#define WaveEditorController_hpp

#include <stdio.h>
#include "BaseController.hpp"
#include "MenuGraph.hpp"
#include "World.hpp"
#include "WaveData.hpp"
#include "TemplateEditorController.hpp"
#include "TemplateWaveEntry.hpp"

enum class WaveEditorState : int {
	START,
	SELECT,
	EDIT,
	DRAG,
	REMOVE,
	DONE,
	TEMPLATE_EDIT,
	NEW_TEMPLATE
};

class WaveEditorController : public BaseController {
private:
	std::shared_ptr<World> _world;

	/** the main node for the level editor
	* connect all related nodes to this node not the scene */
	std::shared_ptr<cugl::Node> _levelEditNode;

	std::shared_ptr<WaveData> _currentWave;

	WaveEditorState _state;

	std::shared_ptr<TemplateEditorController> _templateEditorController;

	std::shared_ptr<GenericAssetManager> _assets;

	std::vector<std::shared_ptr<TemplateWaveEntry>> _templates;

public:
	virtual void attach(Observer* obs);

	virtual void detach(Observer* obs);

	virtual void notify(Event* e);

	/**
	* Update the observer state based on an event from the subject
	*/
	virtual void eventUpdate(Event* e);

	virtual bool update(float timestep, std::shared_ptr<MenuGraph> menuGraph);

	void clearTemplateNodes();

	void clearNodes();

	void setWave(std::shared_ptr<WaveData> wave);

	void updateTemplateNodes();

	void setSceneGraph();

	virtual bool init(std::shared_ptr<cugl::Node> node, std::shared_ptr<GenericAssetManager> assets);

	static std::shared_ptr<WaveEditorController> alloc(std::shared_ptr<cugl::Node> node, std::shared_ptr<GenericAssetManager> assets) {
		std::shared_ptr<WaveEditorController> result = std::make_shared<WaveEditorController>();
		return (result->init(node, assets) ? result : nullptr);
	}

};

#endif /* WaveEditorController_hpp */
