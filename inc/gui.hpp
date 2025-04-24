
#ifndef __GUI_HPP_INCLUDED__
#define __GUI_HPP_INCLUDED__

#include <string>
#include "glpp/vertex_buffer.hpp"
#include "glpp/vertex_array.hpp"
#include "glpp/texture_array.hpp"
#include "glpp/shader_program.hpp"
#include "glpp/uniform_variable.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "SFML/Window/Window.hpp"
#include "SFML/Window/Event.hpp"

struct GuiShaderStage
{
    static std::string VERTEX;
    static std::string FRAGMENT;
};


class Gui
{
public:

    Gui();
    ~Gui();

    void begin(const sf::Window& window);
    void process(sf::Event& event);
    void render();
    void draw();

    static void Description(const char* description);

private:



    gl::VertexBuffer vertexBuffer;
    gl::VertexBuffer elementBuffer;
    gl::VertexArray vertexArray;
    gl::TextureArray textureArray;
    unsigned int textureArrayHandle = 0;
    gl::ShaderProgram shaderProgram;
    gl::UniformVariable textureUniform;
    gl::UniformVariable projectionUniform;
    glm::mat4 projectionMatrix;


    bool windowFocus;
    std::array<bool, 5> mouseDown;




};



#endif
