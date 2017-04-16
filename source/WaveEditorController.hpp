//
//  TemplateEditorController.hpp
//  Canon
//
//  Created by Kenneth Lee on 4/4/17.
//  Copyright � 2017 Game Design Initiative at Cornell. All rights reserved.
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
    COLOR_TOGGLE,
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
    
    /** The child of this node is the node being dragged */
    std::shared_ptr<cugl::Node> _dragNode;
    
    std::shared_ptr<cugl::Label> _labelNode;
    
    int _dragIndex;
    
    bool _dragStart;
    
    bool _wasPressed;
    
    bool _newEntry;
    
    bool _colorChanged;

	std::shared_ptr<WaveData> _currentWave;

	WaveEditorState _state;

	std::shared_ptr<TemplateEditorController> _templateEditorController;
    
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

	void setWave(std::shared_ptr<WaveData> wave);
    
    std::shared_ptr<TemplateWaveEntry> getTemplateWaveEntry(std::string templateKey);

	void updateTemplateNodes();

	void setSceneGraph();
    
    void checkKeyboardInput();
    
    void updateDragAndDrop();
    
    void updateWaveEntryNodes();
    
    std::string getStateAsString();
    
    void createTemplateFile(std::shared_ptr<TemplateWaveEntry> templ);
    
    void waveEntryButtonListenerFunction(const std::string& name, bool down, int index);
    
    void templateButtonListenerFunction(const std::string& name, bool down, int index);

	virtual bool init(std::shared_ptr<cugl::Node> node, std::shared_ptr<World> world);

	static std::shared_ptr<WaveEditorController> alloc(std::shared_ptr<cugl::Node> node, std::shared_ptr<World> world) {
		std::shared_ptr<WaveEditorController> result = std::make_shared<WaveEditorController>();
		return (result->init(node, world) ? result : nullptr);
	}
    
    void updateWorld(std::shared_ptr<World> world);

};

#endif /* WaveEditorController_hpp */
