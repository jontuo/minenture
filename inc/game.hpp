
#ifndef __GAME_HPP_INCLUDED__
#define __GAME_HPP_INCLUDED__

#include "world.hpp"                    // World()
#include <experimental/optional>        // std::experimental::optional<>()
#include <SFML/Window/Event.hpp>

#include "glpp/vertex_buffer.hpp"
#include "glpp/vertex_array.hpp"
#include "glpp/texture_array.hpp"
#include "glpp/shader_program.hpp"
#include "glpp/uniform_variable.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "vertex.hpp"

#include "type.hpp"
#include "component.hpp"
#include <string>
#include "material.hpp"
//#include "definition.hpp"
#include "entt/entity/prototype.hpp"
#include "entity.hpp" // enum EntityType









class Game
{
public:

    // DEBUG
    bool debug;
    bool debugWindow;
    bool collisionVisualization;
    float collisionVisualizationHalfWidth;
    glm::vec4 collisionVisualizationColor;
    
    std::experimental::optional<World> world;

    Game();
    ~Game();

    void prepare();
    void process(sf::Event& event);
    void update(double deltaTime);
    void render();
    void draw();

    void load();
    void loadMaterials();
    void loadDefinitions();
    void loadWorld();
    
    void testWorld();

private:

    // Used to show entities and text.
    std::vector<EntityVertex> entityVertices;
    gl::VertexBuffer entityVertexBuffer;
    gl::VertexArray entityVertexArray; // Vertex: position, coordinate and texture. Soon color too.
    gl::TextureArray entityTextureArray;
    gl::ShaderProgram entityShaderProgram;

    // Used to show entity selection, spatial grid (debugging), and similar.
    std::vector<ShapeVertex> shapeVertices;
    gl::VertexBuffer shapeVertexBuffer;
    gl::VertexArray shapeVertexArray; // Vertex: position and color.
    gl::ShaderProgram shapeShaderProgram;

    gl::UniformVariable viewUniform;
    gl::UniformVariable projectionUniform;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    glm::ivec2 mousePosition;


    
    bool debugMode;
    bool editMode; // If enabled, input is passed to ? instead of Control. Lets you use the mouse, click on things and drag entities around, etc.




    Entity holdingEntity;
    //std::experimental::optional<cp::Shape> mouseShape;
    //cp::Body mouseBody;
    //cp::Pivot mousePivot;
    //cp::Gear mouseGear;
    double mouseSomeFactor = 0.25;
    double mouseOtherFactor = 60.0;
    double mouseRotationFactor = 0.2;

    Entity tmp;

    Entity selectedEntity;





    std::vector<Material> materials;
    std::vector<Definition> definitions;


    /*void renderDot();
    std::array<ShapeVertex, 6> vertizeLine(const glm::vec2& source, const glm::vec2& target, float halfWidth, const glm::vec4& color);
    void renderRectangle();
    void renderPolygon();
    void renderCircle();
    void renderTexture();


    void testing();
    void test();
    void printEntity(Entity entity);

    Entity createParent(cpVect position);
    Entity createChild(const Body& body, glm::vec2 offset, std::uint16_t texture);

    Entity createSubEntity(Entity parentIdentifier, glm::vec2 offset, std::uint16_t texture);
    Entity createFoundation(cpVect position);
    Entity createPistonCylinder(cpVect position);
    //Entity createEntity(EntityType type, cpVect position, cpGroup group = 0);

    Entity matchEntity(Entity entity);
    void attachEntity(Entity source, Entity target);
    void detachEntity(Entity source, Entity target);*/

    /*std::vector<Entity> getFamily(Entity entity);


    Entity createPlayer(cpVect position);
    Entity createRod(cpVect position);
    Entity createCylinder(cpVect position);
    Entity createPiston(cpVect position);

    Entity testWheel(cp::Vector position);
    Entity testVehicle(cp::Vector position);
    void testRoom();
    Entity testStone(cp::Vector position);
    void testStones(cp::Vector position);*/
};


#endif

/*#ifndef __GAME_HPP_INCLUDED__
#define __GAME_HPP_INCLUDED__

#include "world.hpp"
#include <vector>
#include "vertex.hpp"

#include <SFML/Window.hpp>

#include "glpp/glader.hpp"
#include "glpp/vertex_buffer.hpp"
#include "glpp/vertex_array.hpp"
#include "glpp/texture_array.hpp"
#include "glpp/shader_program.hpp"
#include "glpp/uniform_variable.hpp"

#include <experimental/optional>
#include <glm/gtc/matrix_transform.hpp>

#include "gui.hpp"


class Game
{
public:
    Game();
    ~Game();

    void start();
    void stop();

private:

    sf::ContextSettings contextSettings;
    sf::Window window;

    gl::Glader glader;
    gl::VertexBuffer entityVertexBuffer;
    gl::VertexArray entityVertexArray;
    gl::TextureArray entityTextureArray;
    gl::ShaderProgram entityShaderProgram;
    gl::UniformVariable viewUniform;
    gl::UniformVariable projectionUniform;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    Gui gui;
    bool debugWindow;
    bool demoWindow;
    glm::ivec2 mouseScreenPosition;

    std::experimental::optional<Entity> entitySelected;


    std::experimental::optional<World> world;
    std::vector<Vertex> vertices;



    bool run;
    bool pause;

    MicroSecond targetUpdateTime;

    void process();
    void update();
    void render();
    void draw();

    void load();

};

#endif

*/