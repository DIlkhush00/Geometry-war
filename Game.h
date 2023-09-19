#pragma once

#include "Global.h"
#include "Entity.h"
#include "EntityManager.h"

struct PlayerConfig {
	int SR, CR; // shape radius, collision radius
	int FR, FG, FB; // fill color (r, g, b)
	int OR, OG, OB, OT; // outline color (r, g, b), outline thickness
	int V; // vertices( points )
	float S; // speed
};

struct BulletConfig {
	int SR, CR; // shape radius, collision radius
	int FR, FG, FB; // fill color (r, g, b)
	int OR, OG, OB, OT; // outline color (r, g, b), outline thickness
	int V; // vertices( points )
	float S; // speed
	int L; // life span
	int SB; // special bullet
};

struct EnemyConfig {
	int SR, CR; // shape radius, collision radius
	int OR, OG, OB, OT; // outline color (r, g, b), outline thickness
	int VMIN, VMAX; // vertices( points )
	float SMIN, SMAX; // speed
	int L; // life span
	int SI; // spawn interval
};


class Game
{
	sf::RenderWindow m_window;
	sf::Font m_font;
	sf::Text m_scoreText;
	int m_score{ 0 };
	int m_currentFrame{ 0 };
	int m_lastEnemySpawned{ 0 };
	bool m_paused{ false };
	bool m_running{ true };

	PlayerConfig m_playerConfig;
	BulletConfig m_bulletConfig;
	EnemyConfig m_enemyConfig;
	
	EntityManager m_entityManager;

	std::shared_ptr<Entity> m_player;

	void init(const std::string& config);
	void setPaused(bool paused);

	void sLifeSpan();
	void sMovement();
	void sUserInput();
	void sRender();
	void sSpawner();
	void cCollision();

	void spawnPlayer();
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& pos);
	void spawnEnemy();
	void spawnSmallEnemy(std::shared_ptr<Entity> entity);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

public:
	Game(const std::string& path);

	void run();
};

