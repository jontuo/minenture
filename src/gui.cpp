
#include "gui.hpp"
#include "glpp/shader_stage.hpp"
#include "imgui.h"
#include "SFML/Window/Keyboard.hpp"
#include <glm/ext.hpp>

#include <iostream>

/*


    Not sure if io.DisplaySize = ImVec2((float)w, (float)h); will be set correctly at the beginning, since no resize event has been fired.


*/




Gui::Gui():

    vertexBuffer(GL_ARRAY_BUFFER, GL_STREAM_DRAW),
    elementBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STREAM_DRAW),
    vertexArray(),
    textureArray(),
    shaderProgram(),
    mouseDown({{false}})
{



    ImGui::CreateContext();

    //ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.KeyMap[ImGuiKey_Tab] = sf::Keyboard::Tab;                     // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
    io.KeyMap[ImGuiKey_LeftArrow] = sf::Keyboard::Left;
    io.KeyMap[ImGuiKey_RightArrow] = sf::Keyboard::Right;
    io.KeyMap[ImGuiKey_UpArrow] = sf::Keyboard::Up;
    io.KeyMap[ImGuiKey_DownArrow] = sf::Keyboard::Down;
    io.KeyMap[ImGuiKey_PageUp] = sf::Keyboard::PageUp;
    io.KeyMap[ImGuiKey_PageDown] = sf::Keyboard::PageDown;
    io.KeyMap[ImGuiKey_Home] = sf::Keyboard::Home;
    io.KeyMap[ImGuiKey_End] = sf::Keyboard::End;
    io.KeyMap[ImGuiKey_Delete] = sf::Keyboard::Delete;
    io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::Backspace;
    io.KeyMap[ImGuiKey_Enter] = sf::Keyboard::Enter;
    io.KeyMap[ImGuiKey_Escape] = sf::Keyboard::Escape;
    io.KeyMap[ImGuiKey_A] = sf::Keyboard::A;
    io.KeyMap[ImGuiKey_C] = sf::Keyboard::C;
    io.KeyMap[ImGuiKey_V] = sf::Keyboard::V;
    io.KeyMap[ImGuiKey_X] = sf::Keyboard::X;
    io.KeyMap[ImGuiKey_Y] = sf::Keyboard::Y;
    io.KeyMap[ImGuiKey_Z] = sf::Keyboard::Z;

    io.RenderDrawListsFn = NULL;   // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
    io.SetClipboardTextFn = NULL;
    io.GetClipboardTextFn = NULL;
    io.ClipboardUserData = NULL;




    projectionMatrix = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);


    io.DisplaySize = ImVec2(1920.0f, 1080.0f);
    io.DisplayFramebufferScale = ImVec2(1920.0f > 0 ? (1920.0f / 1920.0f) : 0, 1080.0f > 0 ? (1080.0f / 1080.0f) : 0);
    io.DeltaTime = 1.0f/60.0f; // FIX?



    vertexArray.bind();
    vertexBuffer.bind();
    vertexArray.enableAttribute(0);
    vertexArray.setFloatingAttribute(0, 2, GL_FLOAT, false, sizeof(ImDrawVert), (void*)(0)); // position
    vertexArray.enableAttribute(1);
    vertexArray.setFloatingAttribute(1, 2, GL_FLOAT, false, sizeof(ImDrawVert), (void*)(2 * sizeof(float))); // uv
    vertexArray.enableAttribute(2);
    vertexArray.setFloatingAttribute(2, 4, GL_UNSIGNED_BYTE, true, sizeof(ImDrawVert), (void*)(4 * sizeof(float))); // color
    vertexBuffer.unbind();
    vertexArray.unbind();

    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    glGenTextures(1, &textureArrayHandle);
    glBindTexture(GL_TEXTURE_2D, textureArrayHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    io.Fonts->TexID = (void *)(intptr_t)textureArrayHandle;

    gl::ShaderStage vertexShaderStage(GL_VERTEX_SHADER);
    vertexShaderStage.setSource(GuiShaderStage::VERTEX);
    vertexShaderStage.compile();
    shaderProgram.attachStage(vertexShaderStage.getHandle());
    gl::ShaderStage fragmentShaderStage(GL_FRAGMENT_SHADER);
    fragmentShaderStage.setSource(GuiShaderStage::FRAGMENT);
    fragmentShaderStage.compile();
    shaderProgram.attachStage(fragmentShaderStage.getHandle());
    shaderProgram.link();
    //shaderProgram.detachSource(vertexSource);
    //shaderProgram.detachSource(fragmentSource);

    projectionUniform.setLocation(shaderProgram.getHandle(), "projectionMatrix");
    textureUniform.setLocation(shaderProgram.getHandle(), "textureIdentifier");
    

    shaderProgram.bind();
    projectionUniform.setData(glm::value_ptr(projectionMatrix));
    textureUniform.setData(0);
    shaderProgram.unbind();

}




Gui::~Gui()
{    
    if(textureArrayHandle)
    {
        glDeleteTextures(1, &textureArrayHandle);
        ImGui::GetIO().Fonts->TexID = 0;
        textureArrayHandle = 0;
    }

    ImGui::DestroyContext();
}



void Gui::begin(const sf::Window& window)
{
    ImGuiIO& io = ImGui::GetIO();

    io.MousePos = ImVec2((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y);

    for(unsigned int mouseButton = 0; mouseButton < mouseDown.size(); mouseButton++)
    {
        io.MouseDown[mouseButton] = mouseDown[mouseButton] || sf::Mouse::isButtonPressed((sf::Mouse::Button)mouseButton);
        mouseDown[mouseButton] = false;
    }

    ImGui::NewFrame();
}


void Gui::process(sf::Event& event)
{
    ImGuiIO& io = ImGui::GetIO();
    switch(event.type)
    {
        case sf::Event::MouseButtonPressed:
        {
            mouseDown[event.mouseButton.button] = true;

            break;
        }
        case sf::Event::MouseWheelScrolled:
        {
            io.MouseWheel += static_cast<float>(event.mouseWheelScroll.delta);

            break;
        }
        case sf::Event::TextEntered:
        {
            if (event.text.unicode > 0 && event.text.unicode < 0x10000)
            {
                io.AddInputCharacter(static_cast<ImWchar>(event.text.unicode));
            }

            break;
        }
        case sf::Event::KeyPressed:
        case sf::Event::KeyReleased:
        {
            io.KeysDown[event.key.code] = (event.type == sf::Event::KeyPressed);
            io.KeyCtrl = event.key.control;
            io.KeyShift = event.key.shift;
            io.KeyAlt = event.key.alt;

            break;
        }
        case sf::Event::Resized:
        {
            // Update projectionMatrix and io.displayBajs here?
            io.DisplaySize = ImVec2((float)event.size.width, (float)event.size.height);
            io.DisplayFramebufferScale = ImVec2(1920.0f > 0 ? (1920.0f / 1920.0f) : 0, 1080.0f > 0 ? (1080.0f / 1080.0f) : 0);

            break;
        }
        default:
        {
            break;
        }
    }
}




void Gui::render()
{
    ImGui::Render();
}

void Gui::draw()
{
    ImGuiIO& io = ImGui::GetIO();
    ImDrawData* drawData = ImGui::GetDrawData();

    // scale stuff (needed for proper handling of window resize)
    int fb_width = static_cast<int>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = static_cast<int>(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
    {
        return;
    }
    drawData->ScaleClipRects(io.DisplayFramebufferScale);

    //drawData->ScaleClipRects(io.DisplayFramebufferScale);




     // ?
    //(x1=left,y1=top,x2=right,y2=bottom) and NOT as (x1,y1,width,height).



    // scale stuff (needed for proper handling of window resize)
    //int fb_width = static_cast<int>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    //int fb_height = static_cast<int>(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    //if (fb_width == 0 || fb_height == 0) { return; }
    //draw_data->ScaleClipRects(io.DisplayFramebufferScale);

/*  int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);*/



    GLenum last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
    glActiveTexture(GL_TEXTURE0);
    GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_sampler; glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
    GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
    GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
    GLint last_polygon_mode[2]; glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
    GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
    GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
    GLenum last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
    GLenum last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
    GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
    GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
    GLenum last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
    GLenum last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
    GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
    GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);




    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shaderProgram.bind();
    vertexArray.bind();


    // Draw
    for (int n = 0; n < drawData->CmdListsCount; n++)
    {
        const ImDrawList* commands = drawData->CmdLists[n];
        const ImDrawIdx* indexBufferOffset = 0;







        vertexBuffer.bind();
        vertexBuffer.setData((const GLvoid*)commands->VtxBuffer.Data, (GLsizeiptr)commands->VtxBuffer.Size * sizeof(ImDrawVert));
        vertexBuffer.unbind();

        elementBuffer.bind();
        elementBuffer.setData((const GLvoid*)commands->IdxBuffer.Data, (GLsizeiptr)commands->IdxBuffer.Size * sizeof(ImDrawIdx));
        //elementBuffer.unbind();

        for (int cmd_i = 0; cmd_i < commands->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &commands->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(commands, pcmd);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, indexBufferOffset);
            }
            indexBufferOffset += pcmd->ElemCount;
        }
    }

    vertexArray.unbind();
    shaderProgram.unbind();

    glUseProgram(last_program);
    glBindTexture(GL_TEXTURE_2D, last_texture);
    if (glBindSampler) glBindSampler(0, last_sampler);
    glActiveTexture(last_active_texture);
    glBindVertexArray(last_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
    glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
    glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
    if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
    if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
    if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
    glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

std::string GuiShaderStage::VERTEX = R"(
#version 330

layout (location = 0) in vec2 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec4 Color;
uniform mat4 projectionMatrix;

out vec2 Frag_UV;
out vec4 Frag_Color;

void main()
{
   Frag_UV = UV;
   Frag_Color = Color;

   gl_Position = projectionMatrix * vec4(Position.xy,0,1);
}

)";

std::string GuiShaderStage::FRAGMENT = R"(
#version 330

in vec2 Frag_UV;
in vec4 Frag_Color;
uniform sampler2D textureIdentifier;

out vec4 Out_Color;

void main()
{
   Out_Color = Frag_Color * texture(textureIdentifier, Frag_UV.st);
}

)";


void Gui::Description(const char* description)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(description);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

