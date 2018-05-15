#include "Shader.h"
#include "Renderer.h"
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filename) {

	auto* source = parse_shader(filename);
	m_shaderID = create_shader(source);
	delete source;
}

Shader::~Shader() {
	glDeleteProgram(m_shaderID);
}

void Shader::use_program() const{ 
	GLCALL(glUseProgram(m_shaderID)); 
}

int Shader::uniform_location(const char* name) const {
	GLCALL(int location = glGetUniformLocation(m_shaderID, name));
	ASSERT(location != -1);
	return location;
}

ShaderProgramSource* Shader::parse_shader(const std::string& file_path) {
	std::ifstream input(file_path);
	if(!input) {
		std::cout << "Could not find file " << file_path << std::endl;
		input.close();
	}

	enum READ_MODE {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	} current = READ_MODE::NONE;

	std::string line;
	std::stringstream shaders[2];
	while(getline(input, line)) {
		if(line.find("#shader") != std::string::npos) {

			if(line.find("vertex") != std::string::npos) {
				current = READ_MODE::VERTEX;
			} else if(line.find("fragment") != std::string::npos) {
				current = READ_MODE::FRAGMENT;
			}

		} else {
			shaders[static_cast<int> (current)] << line << '\n';

			if(line.find("version") != std::string::npos) {
				if(current == READ_MODE::VERTEX) {
					std::cout << "Vertex shader using GLSL Version " << line.substr(9) << std::endl;
				} else if(current == READ_MODE::FRAGMENT) {
					std::cout << "Fragment shader using GLSL Version " << line.substr(9) << std::endl;
				}
			}
		}
	}

	input.close();

	ShaderProgramSource* read_shader = new ShaderProgramSource;
	read_shader->fragment = shaders[READ_MODE::FRAGMENT].str();
	read_shader->vertex = shaders[READ_MODE::VERTEX].str();
	return read_shader;
}

unsigned Shader::compile_shader(unsigned int type, const std::string& source) {

	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Error compiling " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader: " << std::endl << message << std::endl;
		delete[] message;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned Shader::create_shader(const ShaderProgramSource* source) {

	unsigned program = glCreateProgram();
	unsigned vs = compile_shader(GL_VERTEX_SHADER, source->vertex);
	unsigned fs = compile_shader(GL_FRAGMENT_SHADER, source->fragment);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
