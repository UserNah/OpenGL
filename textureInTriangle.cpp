#include <iostream> 
#include <GL/glew.h> 
#include <gl/freeglut.h> 
#include <math.h> 
#include <vector> 
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include <soil.h>


GLuint vbo, vboo, vboloc;
GLuint col, colloc;
GLuint tx, txloc, uvloc, tex;
void myinit() {
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	tex = SOIL_load_OGL_texture("image4.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	


	GLuint shp, vsh, fsh;
	shp = glCreateProgram();
	vsh = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vshCode =
		"#version 130\n"
		"attribute vec3 pos;"
		"attribute vec2 uv1;"
		"varying vec2 uv;"
		"void main(){"
		"gl_Position=vec4(pos,1);"
		"uv=uv1;}";
	GLint vshlen = strlen(vshCode);
	glShaderSource(vsh, 1, &vshCode, &vshlen);
	glCompileShader(vsh);
	glAttachShader(shp, vsh);

	GLint isCompiled = 0;
	glGetShaderiv(vsh, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vsh, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character 
		 std::vector< GLchar> errorLog(maxLength);
		  glGetShaderInfoLog(vsh, maxLength, &maxLength, &errorLog[0]);
		std::cout << errorLog.data();

	}

	fsh = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fshCode = "#version 130\n"
		"uniform sampler2D tx;"
		"varying vec2 uv;"
		"void main(){"
		"gl_FragColor=vec4(texture(tx,uv));}";

	GLint fshlen = strlen(fshCode);
	glShaderSource(fsh, 1, &fshCode, &fshlen);
	glCompileShader(fsh);

	glGetShaderiv(fsh, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fsh, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character 
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(fsh, maxLength, &maxLength, &errorLog[0]);
		std::cout << errorLog.data();

	}

	glAttachShader(shp, fsh);

	glValidateProgram(shp);
	glLinkProgram(shp);
	glUseProgram(shp);
	//glClearColor(0.5, 0.5, 1, 1);
	float positions[] = {  -1,-1,0,   1,-1,0,   0,1,0  };//{nizhnii-levyi,   nizhnii-pravyi,   verhnyi-tsentr};
	float uv[] = { 0,0, 1,0, 0.5,1 };
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, &positions[0], GL_STATIC_DRAW);

	vboloc = glGetAttribLocation(shp, "pos");
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(vboloc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glGenBuffers(1, &vboo);
	glBindBuffer(GL_ARRAY_BUFFER, vboo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, &uv[0], GL_STATIC_DRAW);
	uvloc = glGetAttribLocation(shp, "uv1");
	glBindBuffer(GL_ARRAY_BUFFER, vboo);
	glVertexAttribPointer(uvloc, 2, GL_FLOAT, GL_FALSE, 0, 0);

	txloc = glGetUniformLocation(shp, "tx");
	glUniform1i(txloc, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);


}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnableVertexAttribArray(vboloc);
	glEnableVertexAttribArray(uvloc);


	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(vboloc);
	glDisableVertexAttribArray(uvloc);
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("window");

	glewInit();

	myinit();

	glutDisplayFunc(display);
	glutIdleFunc(display);

	glutMainLoop();

	return 0;
}