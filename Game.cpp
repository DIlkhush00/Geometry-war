#include "Game.h"
#include "Global.h"


Game::Game(const std::string& config)
{
	init(config);
}

void Game::run()
{
	while (m_running)
	{
		m_entityManager.update();
		if (!m_paused)
		{
			sLifeSpan();
			sMovement();
			sSpawner();
			cCollision();
		}
		sUserInput();
		sRender();

		m_currentFrame++;
	}
	
}

void Game::init(const std::string& config)
{
	
	std::fstream input{ config };
	
	if (!input.is_open())
	{
		std::cout << "Failed to open: " << config << '\n';
		exit(-1);
	}

	std::string identifier{};
	while (input >> identifier)
	{

		if (identifier == "Window")
		{
			unsigned int width{};
			input >> width;

			unsigned int height{};
			input >> height;

			int frameLimit{};
			input >> frameLimit;

			int fullScreen{};
			input >> fullScreen;
			if (fullScreen == 0)
			{
				m_window.create(sf::VideoMode(width, height), "Geometry Wars", sf::Style::Close);
				m_window.setFramerateLimit(frameLimit);
			}
			else if (fullScreen == 1)
			{
				auto fullscreenMode{ sf::VideoMode::getFullscreenModes() };
				//fullscreenMode[0] is the most compatible mode for fullscreen on this device
				m_window.create(fullscreenMode[0], "Geometry Wars", sf::Style::Fullscreen);
				m_window.setFramerateLimit(frameLimit);
			}
		}
		else if (identifier == "Font")
		{
			std::string filepath{};
			input >> filepath;
			if (!m_font.loadFromFile(filepath))
			{
				std::cerr << "Failed to load font. Filepath: " << filepath;
			}

			m_scoreText.setFont(m_font);

			int fontSize{};
			input >> fontSize;
			m_scoreText.setCharacterSize(fontSize);

			sf::Vector3<sf::Uint16> RGB{};
			input >> RGB.x;
			input >> RGB.y;
			input >> RGB.z;
			m_scoreText.setFillColor(sf::Color(RGB.x, RGB.y, RGB.z));
		}
		else if (identifier == "Player")
		{
			int shapeRadius{};
			input >> shapeRadius;
			m_playerConfig.SR = shapeRadius;

			int collisionRadius{};
			input >> collisionRadius;
			m_playerConfig.CR = collisionRadius;

			float speed{};
			input >> speed;
			m_playerConfig.S = speed;

			sf::Vector3<sf::Uint16> fillColor{};
			input >> fillColor.x;
			input >> fillColor.y;
			input >> fillColor.z;
			m_playerConfig.FR = fillColor.x;
			m_playerConfig.FG = fillColor.y;
			m_playerConfig.FB = fillColor.z;

			sf::Vector3<sf::Uint16> outlineColor{};
			input >> outlineColor.x;
			input >> outlineColor.y;
			input >> outlineColor.z;
			m_playerConfig.OR = outlineColor.x;
			m_playerConfig.OG = outlineColor.y;
			m_playerConfig.OB = outlineColor.z;

			int outlineThickness{};
			input >> outlineThickness;
			m_playerConfig.OT = outlineThickness;

			int vertices{};
			input >> vertices;
			m_playerConfig.V = vertices;
		}
		else if (identifier == "Enemy")
		{
			int shapeRadius{};
			input >> shapeRadius;
			m_enemyConfig.SR = shapeRadius;

			int collisionRadius{};
			input >> collisionRadius;
			m_enemyConfig.CR = collisionRadius;

			float minSpeed{};
			input >> minSpeed;
			m_enemyConfig.SMIN = minSpeed;

			float maxSpeed{};
			input >> maxSpeed;
			m_enemyConfig.SMAX = maxSpeed;

			sf::Vector3<sf::Uint16> outlineColor{};
			input >> outlineColor.x;
			input >> outlineColor.y;
			input >> outlineColor.z;
			m_enemyConfig.OR = outlineColor.x;
			m_enemyConfig.OG = outlineColor.y;
			m_enemyConfig.OB = outlineColor.z;

			int outlineThickness{};
			input >> outlineThickness;
			m_enemyConfig.OT = outlineThickness;

			int minVertices{};
			input >> minVertices;
			m_enemyConfig.VMIN = minVertices;

			int maxVertices{};
			input >> maxVertices;
			m_enemyConfig.VMAX = maxVertices;

			int smallLifespan{};
			input >> smallLifespan;
			m_enemyConfig.L = smallLifespan;

			int spawnInterval{};
			input >> spawnInterval;
			m_enemyConfig.SI = spawnInterval;
		}
		else if (identifier == "Bullet")
		{
			int shapeRadius{};
			input >> shapeRadius;
			m_bulletConfig.SR = shapeRadius;

			int collisionRadius{};
			input >> collisionRadius;
			m_bulletConfig.CR = collisionRadius;

			float speed{};
			input >> speed;
			m_bulletConfig.S = speed;

			sf::Vector3<sf::Uint16> fillColor{};
			input >> fillColor.x;
			input >> fillColor.y;
			input >> fillColor.z;
			m_bulletConfig.FR = fillColor.x;
			m_bulletConfig.FG = fillColor.y;
			m_bulletConfig.FB = fillColor.z;

			sf::Vector3<sf::Uint16> outlineColor{};
			input >> outlineColor.x;
			input >> outlineColor.y;
			input >> outlineColor.z;
			m_bulletConfig.OR = outlineColor.x;
			m_bulletConfig.OG = outlineColor.y;
			m_bulletConfig.OB = outlineColor.z;

			int outlineThickness{};
			input >> outlineThickness;
			m_bulletConfig.OT = outlineThickness;

			int shapeVertices{};
			input >> shapeVertices;
			m_bulletConfig.V = shapeVertices;

			int lifespan{};
			input >> lifespan;
			m_bulletConfig.L = lifespan;

			int specialBulletAmount{};
			input >> specialBulletAmount;
			m_bulletConfig.SB = specialBulletAmount;
		}
	}

	m_scoreText.setPosition(0, 0);
	m_scoreText.setString(std::to_string(m_score));

	spawnPlayer();
}

void Game::setPaused(bool paused)
{
	m_paused = paused;
}

void Game::sMovement()
{
	// player movements
	Vec2 playerVelocity;

	if (m_player->cInput->up)
	{
		playerVelocity.y -= m_playerConfig.S;
	}
	if (m_player->cInput->down)
	{
		playerVelocity.y += m_playerConfig.S;
	}
	if (m_player->cInput->left)
	{
		playerVelocity.x -= m_playerConfig.S;
	}
	if (m_player->cInput->right)
	{
		playerVelocity.x += m_playerConfig.S;
	}

	for (auto entity : m_entityManager.getEntities())
	{
		if (entity->tag() == "player")
		{
			m_player->cTransform->pos += playerVelocity;
			entity->cTransform->angle += 2.0f;
			entity->cShape->circle.setRotation(entity->cTransform->angle);
		}
		else if (entity->cTransform)
		{
			entity->cTransform->pos += entity->cTransform->velocity;
			entity->cTransform->angle += 2.0f;
			entity->cShape->circle.setRotation(entity->cTransform->angle);
		}
	}
}

void Game::sLifeSpan()
{
	for (auto e : m_entityManager.getEntities())
	{
		if (!e->cLifespan) { continue; }

		if (e->cLifespan->remaining > 0)
		{
			--e->cLifespan->remaining;
		}
		if (e->isActive() && e->cLifespan->remaining > 0)
		{
			float alphaMultiplier{ static_cast<float>(e->cLifespan->remaining) / static_cast<float>(e->cLifespan->total) };

			auto fillColor{ e->cShape->circle.getFillColor() };
			sf::Color newFillColor{ fillColor.r,fillColor.g,fillColor.b, static_cast<sf::Uint8>(255 * alphaMultiplier) };
			e->cShape->circle.setFillColor(newFillColor);

			auto outlineColor{ e->cShape->circle.getOutlineColor() };
			sf::Color newOutlineColor{ outlineColor.r,outlineColor.g,outlineColor.b, static_cast<sf::Uint8>(255 * alphaMultiplier) };
			e->cShape->circle.setOutlineColor(newOutlineColor);
		}
		else if (e->cLifespan->remaining <= 0)
		{
			e->destroy();
		}
	}
}

void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		switch (event.type)
		{

		case sf::Event::Closed:
		{
			m_running = false;
			break;
		}
		case sf::Event::KeyPressed:
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				m_running = false;
			}
			else if (event.key.code == sf::Keyboard::W)
			{
				m_player->cInput->up = true;
			}
			else if (event.key.code == sf::Keyboard::A)
			{
				m_player->cInput->left = true;
			}
			else if (event.key.code == sf::Keyboard::D)
			{
				m_player->cInput->right = true;
			}
			else if (event.key.code == sf::Keyboard::S)
			{
				m_player->cInput->down = true;
			}
			else if (event.key.code == sf::Keyboard::P)
			{
				m_paused ? setPaused(false) : setPaused(true);
				std::cout << "Pause Command" << std::endl;

			}

			break;
		}
		case sf::Event::KeyReleased:
		{
			if (event.key.code == sf::Keyboard::A) { m_player->cInput->left = false; }
			else if (event.key.code == sf::Keyboard::D) { m_player->cInput->right = false; }
			else if (event.key.code == sf::Keyboard::W) { m_player->cInput->up = false; }
			else if (event.key.code == sf::Keyboard::S) { m_player->cInput->down = false; }

			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			if (event.key.code == sf::Mouse::Left)
			{
				m_player->cInput->mouseLeft = true;
			}
			if (event.key.code == sf::Mouse::Right)
			{
				m_player->cInput->mouseRight = true;
			}
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			if (event.key.code == sf::Mouse::Left)
			{
				m_player->cInput->mouseLeft = false;
			}
			if (event.key.code == sf::Mouse::Right)
			{
				m_player->cInput->mouseRight = false;
			}
			break;
		}
		default: break;
		}
	}
}

void Game::sRender()
{
	m_window.clear();
	for (auto entity : m_entityManager.getEntities())
	{
		entity->cShape->circle.setPosition(entity->cTransform->pos.x, entity->cTransform->pos.y);
		m_window.draw(entity->cShape->circle);
	}
	m_window.draw(m_scoreText);
	m_window.display();
}

void Game::sSpawner()
{
	if ((m_currentFrame - m_lastEnemySpawned) >= m_enemyConfig.SI)
	{
		spawnEnemy();
	}

	// bullet movements
	if (m_player->cInput->mouseLeft)
	{
		sf::Mouse mouse;
		Vec2 mousePos(mouse.getPosition(m_window).x, mouse.getPosition(m_window).y);
		std::cout << '(' << mousePos.x << ',' << mousePos.y << ')' << std::endl;
		spawnBullet(m_player, mousePos);
		m_player->cInput->mouseLeft = false;
	}

	if (m_player->cInput->mouseRight)
	{
		spawnSpecialWeapon(m_player);
		m_player->cInput->mouseRight = false;
	}
}

void Game::cCollision()
{
	// Handling player's collision with walls
	Vec2 playerCurrentPosition( m_player->cTransform->pos.x, m_player->cTransform->pos.y );
	if (playerCurrentPosition.y <= 0)
	{
		m_player->cTransform->pos.y += m_playerConfig.S;
	}
	else if (playerCurrentPosition.y > m_window.getSize().y)
	{
		m_player->cTransform->pos.y -= m_playerConfig.S;
	}

	if (playerCurrentPosition.x <= 0)
	{
		m_player->cTransform->pos.x += m_playerConfig.S;
	}
	else if (playerCurrentPosition.x > m_window.getSize().x)
	{
		m_player->cTransform->pos.x -= m_playerConfig.S;
	}

	// Handling collision of bullets with enemies
	for (auto bullet : m_entityManager.getEntities("bullet"))
	{
		for (auto enemy : m_entityManager.getEntities("enemy"))
		{
			Vec2 diff(  bullet->cTransform->pos.x - enemy->cTransform->pos.x,
						bullet->cTransform->pos.y - enemy->cTransform->pos.y		);
			
			double diffSQ{ (diff.x * diff.x) + (diff.y * diff.y) };
			double collisionDiffSQ{ (bullet->cCollision->radius + enemy->cCollision->radius) * (bullet->cCollision->radius + enemy->cCollision->radius)};

			if (diffSQ < collisionDiffSQ)
			{
				m_score += enemy->cScore->score;
				m_scoreText.setString(std::to_string(m_score));

				spawnSmallEnemy(enemy);

				bullet->destroy();
				enemy->destroy();

				break;
			}
		}
	}

	// Handling collision between player and enemy
	for (auto player : m_entityManager.getEntities("player"))
	{
		for (auto enemy : m_entityManager.getEntities("enemy"))
		{
			Vec2 diff(player->cTransform->pos.x - enemy->cTransform->pos.x,
				player->cTransform->pos.y - enemy->cTransform->pos.y);

			double diffSQ{ (diff.x * diff.x) + (diff.y * diff.y) };
			double collisionDiffSQ{ (player->cCollision->radius + enemy->cCollision->radius) * (player->cCollision->radius + enemy->cCollision->radius) };

			if (diffSQ < collisionDiffSQ)
			{
				m_score = 0;
				m_scoreText.setString(std::to_string(m_score));

				player->destroy();
				enemy->destroy();

				spawnPlayer();

				break;
			}
		}
		// for small enemies
		for (auto enemy : m_entityManager.getEntities("smallEnemy"))
		{

			Vec2 diff{ enemy->cTransform->pos.x - player->cTransform->pos.x , enemy->cTransform->pos.y - player->cTransform->pos.y };

			double collisionRadiusSQ{ (player->cCollision->radius + enemy->cCollision->radius) * (player->cCollision->radius + enemy->cCollision->radius) };
			double distSQ{ (diff.x * diff.x) + (diff.y * diff.y) };

			if (distSQ < collisionRadiusSQ)
			{
				//makes sure the player is alive so it doesnt spawn 2 players
				if (player->isActive())
				{
					m_score /= 2;
					m_scoreText.setString(std::to_string(m_score));

					player->destroy();
					enemy->destroy();
					spawnPlayer();
				}
			}
		}
	}

	// Handling Walls Collision for entities
	for (auto e : m_entityManager.getEntities())
	{
		if (e->tag() == "bullet" || e->tag() == "enemy")
		{
			if (e->cTransform->pos.x + e->cCollision->radius > m_window.getSize().x)
			{
				e->cTransform->velocity.x *= -1;
			}
			else if (e->cTransform->pos.x - e->cCollision->radius < 0)
			{
				e->cTransform->velocity.x *= -1;
			}
			if (e->cTransform->pos.y + e->cCollision->radius > m_window.getSize().y)
			{
				e->cTransform->velocity.y *= -1;
			}
			else if (e->cTransform->pos.y - e->cCollision->radius < 0)
			{
				e->cTransform->velocity.y *= -1;
			}
		}
	}
}

void Game::spawnPlayer()
{
	auto player = m_entityManager.addEntity("player");

	Vec2 pos(m_window.getSize().x / 2.0, m_window.getSize().y / 2.0);
	Vec2 velo(0, 0);
	player->cTransform = std::make_shared<CTransform>(pos, velo, 0.0f);

	sf::Color fillColor(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB);
	sf::Color outlineColor(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB);

	player->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, fillColor, outlineColor, m_playerConfig.OT);

	player->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);

	player->cInput = std::make_shared<CInput>();

	m_player = player;

}

void Game::spawnBullet(std::shared_ptr<Entity> shooter, const Vec2& mousePos)
{
	auto bullet = m_entityManager.addEntity("bullet");
	
	bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, 
						sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
						sf::Color(m_bulletConfig.OR, m_bulletConfig.OG,
						m_bulletConfig.OB), m_bulletConfig.OT);

	bullet->cLifespan = std::make_shared<CLifeSpan>(m_bulletConfig.L);
	bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);

	Vec2 diff{ mousePos.x - shooter->cTransform->pos.x, mousePos.y - shooter->cTransform->pos.y };
	diff.normalise();

	Vec2 velocity(m_bulletConfig.S * diff.x, m_bulletConfig.S * diff.y);


	bullet->cTransform = std::make_shared<CTransform>(shooter->cTransform->pos, velocity, 0.0);
}

void Game::spawnEnemy()
{
	auto entity = m_entityManager.addEntity("enemy");

	int minPosX{ m_enemyConfig.CR };
	int maxPosX{ static_cast<int>(m_window.getSize().x - m_enemyConfig.CR) };

	int minPosY{ m_enemyConfig.CR };
	int maxPosY{ static_cast<int>(m_window.getSize().y - m_enemyConfig.CR) };

	Vec2 randPos
	{
		static_cast<double>(minPosX + (rand() % (maxPosX - minPosX + 1))),
		static_cast<double>(minPosY + (rand() % (maxPosY - minPosY + 1)))
	};

	int randSpeed
	{
		static_cast<int>(m_enemyConfig.SMIN + (rand() % static_cast<int>(m_enemyConfig.SMAX - m_enemyConfig.SMIN + 1)))
	};

	// making sure randSpeed is not zero

	while (randSpeed == 0)
	{
		randSpeed = static_cast<int>(m_enemyConfig.SMIN + (rand() % static_cast<int>(m_enemyConfig.SMAX - m_enemyConfig.SMIN + 1)));
	};

	entity->cTransform = std::make_shared<CTransform>(randPos, Vec2(randSpeed, randSpeed), 0.0f);

	entity->cScore = std::make_shared<CScore>(100);
	
	int vertices{ static_cast<int>(m_enemyConfig.VMIN + (rand() % (m_enemyConfig.VMAX - m_enemyConfig.VMIN + 1))) };

	sf::Color fillColor
	{
		static_cast<sf::Uint8>(rand() % (1 + 255)),
		static_cast<sf::Uint8>(rand() % (1 + 255)),
		static_cast<sf::Uint8>(rand() % (1 + 255))
	};

	sf::Color outlineColor
	{
		static_cast<sf::Uint8>(m_enemyConfig.OR),
		static_cast<sf::Uint8>(m_enemyConfig.OG),
		static_cast<sf::Uint8>(m_enemyConfig.OB)
	};

	int outlineThickness{ m_enemyConfig.OT };

	entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, vertices, fillColor, outlineColor, outlineThickness);
	
	// Here goes collision
	entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);

	// record when the last time enemy spawned
	m_lastEnemySpawned = m_currentFrame;
}

void Game::spawnSmallEnemy(std::shared_ptr<Entity> parent)
{

	//Spawn a number of small enemies equal to the number of vertices of the original
	size_t vertices{ parent->cShape->circle.getPointCount() };

	Vec2 ParentPos{ parent->cTransform->pos.x ,parent->cTransform->pos.y };

	Vec2 normalizedParentPos{ Vec2::normalise(ParentPos)};

	//Set each enemy to the same color as the original, half the size
	sf::Color parentFill{ parent->cShape->circle.getFillColor() };
	sf::Color parentOutline{ parent->cShape->circle.getOutlineColor() };
	float parentThickness{ parent->cShape->circle.getOutlineThickness() };

	float smallEnemyRadius{ parent->cShape->circle.getRadius() * 0.5f };
	float smallEnemyCollisionRadius{ parent->cCollision->radius * 0.5f };

	float angle{ 0 };

	for (size_t i{ 0 }; i < vertices; ++i)
	{
		auto e = m_entityManager.addEntity("smallEnemy");

		//Small enemies are worth double points of the original enemy
		e->cScore = std::make_shared<CScore>(parent->cScore->score * 2);

		e->cShape = std::make_shared<CShape>(smallEnemyRadius, vertices, parentFill, parentOutline, parentThickness);

		e->cCollision = std::make_shared<CCollision>(smallEnemyCollisionRadius);

		e->cLifespan = std::make_shared<CLifeSpan>(m_enemyConfig.L);

		//Calculate the velocity

		double radians{ angle * 3.1415 / 180.0 };

		Vec2 velocity
		{
			std::cos(radians) * normalizedParentPos.x + std::sin(radians) * normalizedParentPos.y,
			std::sin(radians) * normalizedParentPos.x - std::cos(radians) * normalizedParentPos.y,
		};

		//Get the lenght of the vector 
		float L{ sqrtf(velocity.x * velocity.x + velocity.y * velocity.y) };

		//Normalize the vector to get an unit vector
		Vec2 normalizedVelocity{ velocity.x / L,velocity.y / L };

		//Scales the normalized vertor by the parents velocity
		Vec2 newVelocity{ normalizedVelocity.x * parent->cTransform->velocity.x, normalizedVelocity.y * parent->cTransform->velocity.y };

		e->cTransform = std::make_shared<CTransform>(ParentPos, newVelocity, 0);

		angle += 360 / vertices;
	}

}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	float angle{ 0 };

	for (int j{ 0 }; j < m_bulletConfig.SB; ++j)
	{
		auto e = m_entityManager.addEntity("bullet");

		e->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V,
			sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
			sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);

		e->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);

		e->cLifespan = std::make_shared<CLifeSpan>(m_bulletConfig.L);


		Vec2 normalizedPos{ Vec2::normalise(m_player->cTransform->pos) };

		//Calculate the velocity

		double radians{ angle * 3.14 / 180.0 };

		Vec2 velocity
		{
			std::cos(radians) * normalizedPos.x + std::sin(radians) * normalizedPos.y,
			std::sin(radians) * normalizedPos.x - std::cos(radians) * normalizedPos.y,
		};

		//Get the lenght of the vector 
		double L{ velocity.length() };

		//Normalize the vector to get an unit vector
		Vec2 normalizedVelocity{ velocity.x / L,velocity.y / L };

		//Scales the normalized vertor by the parents velocity
		Vec2 newVelocity{ normalizedVelocity.x * m_bulletConfig.S, normalizedVelocity.y * m_bulletConfig.S };

		e->cTransform = std::make_shared<CTransform>(m_player->cTransform->pos, newVelocity, 0);

		angle += 360 / m_bulletConfig.SB;
	}
}