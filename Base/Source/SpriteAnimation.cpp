#include "SpriteAnimation.h"
#include "GL\glew.h"
#include "Vertex.h"

SpriteAnimation::SpriteAnimation()
{

}

SpriteAnimation::SpriteAnimation(const std::string &meshName, int row, int col) : Mesh(meshName), m_row(row), m_col(col), m_currentTime(0), m_currentFrame(0), m_playCount(0)
{
	m_anim = NULL;
}

SpriteAnimation::~SpriteAnimation()
{
	if(m_anim)
		delete m_anim;
}

void SpriteAnimation::Update(double dt)
{
	if(m_anim)
	{
		m_currentTime += dt;
		int numFrame = (m_anim->endFrame - m_anim->startFrame + 1);
		float frameTime = m_anim->animTime / numFrame;
		
		if(m_anim->endFrame < (m_anim->startFrame + (m_currentTime/frameTime)))
		{
			m_currentFrame = m_anim->endFrame;
		}
		else
		{
			m_currentFrame = (m_anim->startFrame + (m_currentTime/frameTime));
		}

		if(m_currentTime > m_anim->animTime)
		{
			m_anim->ended = true;

			if(m_playCount + 1 < m_anim->repeatCount || m_anim->repeatCount == 0)
			{
				++m_playCount;
				m_currentTime = 0;
				m_currentFrame = m_anim->startFrame;
			}
		}
	}
}

void SpriteAnimation::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
	

	
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	{
		glDisableVertexAttribArray(3);
	}
}
