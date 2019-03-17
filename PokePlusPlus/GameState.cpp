#include "GameState.h"


// +--------------------------+
// | Constructor / Destructor |
// +--------------------------+
GameState::GameState(sf::RenderWindow *window, std::map<std::string, int> *supportedKeys, std::stack<State *> *states)
	: State(window, supportedKeys, states) {
	this->initKeybinds();
	this->initTextures();
	this->initPlayer();
}


GameState::~GameState() {
	delete this->player;
}


// +------------------------+
// | Pure Virtual Functions |
// +------------------------+
void GameState::update(const float &dt) {
	// We always pass deltaTime into these functions to keep
	// the framerate and tickrate consistent across all computers
	this->updateInput(dt);

	this->player->update(dt);
}

void GameState::render(sf::RenderTarget *target) {
	// Make sure that the primary rendering target is the main window
	if (!target) {
		target = this->window;
	}

	this->player->render(this->window);
}

void GameState::updateInput(const float &dt) {
	// Update the player input for movement, this works based on the keybindings
	// we set up in the initKeybinds function which reads from a file
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_UP")))) {
		this->player->move(dt, 0.0f, -1.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN")))) {
		this->player->move(dt, 0.0f, 1.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_LEFT")))) {
		this->player->move(dt, -1.0f, 0.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_RIGHT")))) {
		this->player->move(dt, 1.0f, 0.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE")))) {
		this->endState();
	}
}


// +--------------+
// | Initializers |
// +--------------+
void GameState::initKeybinds() {
	std::ifstream keys("Settings/gamestate_keybinds.ini");

	// Initialize the keybindings from a file instead of hard
	// coding it, just reads them into a dictionary
	if (keys.is_open()) {
		std::string key = "";
		std::string key_value = "";

		while (keys >> key >> key_value) {
			this->keybinds[key] = this->supportedKeys->at(key_value);
		}
	}

	keys.close();
}

void GameState::initTextures() {
	if (!this->textures["PLAYER_IDLE"].loadFromFile("Sprites/clefairy-idle.png")) {
		std::cerr << "Could not load player texture!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void GameState::initPlayer() {
	this->player = new Player(0, 0, &this->textures["PLAYER_IDLE"]);
}
