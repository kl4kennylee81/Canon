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
#include <cugl/cugl.h>
#include "BaseController.hpp"
#include "MenuGraph.hpp"
#include "World.hpp"
#include "WaveData.hpp"
#include "TemplateWaveEntry.hpp"

enum class WaveEditorState : int {
	START,
	SELECT,
	DRAG,
	REMOVE,
    COLOR_TOGGLE,
    AI_TOGGLE,
	DONE,
	NEW_TEMPLATE,
    REFRESH,
    TEMPLATE_TOGGLE,
    ZONE_TOGGLE
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
    
    std::string _waveKey;
    
    bool _dragStart;
    
    bool _wasPressed;
    
    bool _newEntry;
    
    bool _colorChanged;
    
    bool _zoneChanged;
    
    bool _aiChanged;
    
    int _removeIndex;
    
    bool _entryRemoved;
    
    bool _showTemplates;

	std::shared_ptr<WaveData> _currentWave;

	WaveEditorState _state;
    
    WaveEditorState _prevState;
    
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

    std::vector<std::shared_ptr<TemplateWaveEntry>> getTemplates() {
        return _templates;
    }
    
    void setWave(std::string key, std::shared_ptr<WaveData> wave);
    
    std::shared_ptr<TemplateWaveEntry> getTemplateWaveEntry(std::string templateKey);

	void updateTemplateNodes();

	void setSceneGraph();
    
    cugl::Vec2 getAnimationScale(std::string objectKey,bool isNonUniform);
    
    void checkKeyboardInput();
    
    void updateDragAndDrop();
    
    void updateWaveEntryNodes();
    
    void refreshTemplates();
    
    std::string getStateAsString();
    
    void setTemplates(std::vector<std::string> templates);
    
    void createTemplateFile(std::shared_ptr<TemplateWaveEntry> templ);
    
    void waveEntryButtonListenerFunction(const std::string& name, bool down, int index);
    
    void templateButtonListenerFunction(const std::string& name, bool down, int index);

    std::shared_ptr<cugl::Button> getButtonFromTemplate(float x, float y, std::shared_ptr<TemplateWaveEntry> templ, ElementType color);
    
    std::shared_ptr<cugl::Node> createZoneNode(float x, float y,std::string zoneKey, ElementType parentColor);

	virtual bool init(std::shared_ptr<cugl::Node> node, std::shared_ptr<World> world);

	static std::shared_ptr<WaveEditorController> alloc(std::shared_ptr<cugl::Node> node, std::shared_ptr<World> world) {
		std::shared_ptr<WaveEditorController> result = std::make_shared<WaveEditorController>();
		return (result->init(node, world) ? result : nullptr);
	}
    
    void updateWorld(std::shared_ptr<World> world);

};

#endif /* WaveEditorController_hpp */
