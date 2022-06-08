//#include "SceneIciclePlot.h"
//
//SceneIciclePlot::SceneIciclePlot(Shader* shader, SpaceReclaimingIciclePlot plot, int screenWidth, int screenHeight) 
//	: Scene(shader, plot.getVertexDataArray(), plot.getVertexDataArraySize(), screenWidth, screenHeight) 
//{
//	this->VAO = 0;
//	this->VBO = 0;
//
//	//unsigned int VBO, VAO;
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeData, data, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	
//	  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);  //<------------------only this changed
//	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
//	this->frameBuffer = 0;
//	glGenFramebuffers(1, &frameBuffer);
//
//	// Create new texture
//	this->textureBuffer = 0;
//	glGenTextures(1, &textureBuffer);
//	glBindTexture(GL_TEXTURE_2D, textureBuffer);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthScreen, heightScreen, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//	//create renderbuffer object for depth and stencil attachment
//	unsigned int rbo;
//	glGenRenderbuffers(1, &rbo);
//	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, widthScreen, heightScreen); // use a single renderbuffer object for both a depth AND stencil buffer.
//
//	//bind framebuffer and attach texture to renderbufffer
//	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
//
//
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}
//
//void SceneIciclePlot::updateScene()
//{
//	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
//	glEnable(GL_DEPTH_TEST);
//
//	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	//draw scene as usual
//	shader->use();
//	glBindVertexArray(VAO);
//	glDrawArrays(GL_TRIANGLES, 0, 3);
//	// glDrawArrays(GL_QUADS, 0, 8*QUAD_PRECISION);
//
//	//now we read the 'offscreen' framebuffer into the normal fb and are able to see it on the screen.
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glDisable(GL_DEPTH_TEST);
//}
