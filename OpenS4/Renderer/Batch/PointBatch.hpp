#pragma once

#include "../OpenGL.hpp"

namespace OpenS4::Renderer {
class PointBatch {
   public:
    GLuint m_vertexArrayObject = 0;

    GLuint m_attributes[3] = {0};
    u64 m_attributeSize[3] = {0};

    u64 m_numberOfVertices;

    void setAttribute(u64 attrID, const std::vector<float>& attribute,
                      u64 valuesPerAttribute) {
        if (m_attributes[attrID] == 0) {
            glGenBuffers(1, &m_attributes[attrID]);
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_attributes[attrID]);
        if (m_attributeSize[attrID] < attribute.size()) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * attribute.size(),
                         attribute.data(), GL_DYNAMIC_DRAW);
            m_attributeSize[attrID] = attribute.size();
        } else {
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            sizeof(float) * attribute.size(), attribute.data());
        }
        glVertexAttribPointer(attrID, valuesPerAttribute, GL_FLOAT, GL_FALSE, 0,
                              0);
    }

    void setAttribute(u64 attrID, float* attribute, u64 attributeLength,
                      u64 valuesPerAttribute) {
        if (m_attributes[attrID] == 0) {
            glGenBuffers(1, &m_attributes[attrID]);
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_attributes[attrID]);
        if (m_attributeSize[attrID] < attributeLength) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * attributeLength,
                         attribute, GL_DYNAMIC_DRAW);
            m_attributeSize[attrID] = attributeLength;
        } else {
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * attributeLength,
                            attribute);
        }
        glVertexAttribPointer(attrID, valuesPerAttribute, GL_FLOAT, GL_FALSE, 0,
                              0);
    }

    void bindVAO() {
        if (m_vertexArrayObject == 0) {
            glGenVertexArrays(1, &m_vertexArrayObject);
        }
        glBindVertexArray(m_vertexArrayObject);
    }

   public:
    PointBatch() {}
    ~PointBatch() {
        for (int i = 0; i < 3; i++) {
            if (m_attributes[i]) glDeleteBuffers(1, &m_attributes[i]);
        }

        if (m_vertexArrayObject != 0)
            glDeleteVertexArrays(1, &m_vertexArrayObject);
    }

    void draw() {
        glBindVertexArray(m_vertexArrayObject);
        glDrawArrays(GL_POINTS, 0, m_numberOfVertices);
        glBindVertexArray(0);
    }

    void updateData(const std::vector<float>& xy, u64 nXY,
                    const std::vector<float>& color, u64 nColor) {
        bindVAO();

        setAttribute(0, xy, nXY);
        setAttribute(2, color, nColor);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_numberOfVertices = xy.size() / nXY;
    }
};

}  // namespace OpenS4::Renderer
