#pragma once
#include "Engine/Prerequisites.h"

/**
 * @brief Componente que almacena la información geométrica de un modelo 3D.
 *
 * Contiene los datos crudos (vértices e índices) necesarios para construir
 * la malla, así como su matriz de transformación local. Es la estructura de
 * datos que posteriormente leerá la clase Buffer para pasarlos a la GPU.
 */
class
MeshComponent {
public:
  /**
   * @brief Constructor por defecto.
   *
   * Inicializa el número de vértices e índices en 0.
   */
  MeshComponent() : m_numVertex(0), m_numIndex(0) {}

public:
  std::string m_name; ///< Nombre identificador de la malla (útil para debugging o búsqueda).

  /**
   * @brief Matriz de transformación local de la malla.
   *
   * Por defecto se inicializa como una matriz Identidad. Define la posición,
   * rotación y escala de la malla respecto al espacio de su modelo original.
   */
  DirectX::XMFLOAT4X4 m_localTransform = DirectX::XMFLOAT4X4(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f);

  /**
   * @brief Colección de vértices que conforman la geometría.
   */
  std::vector<Vertex> m_vertex;

  // std::vector<SkyboxVertex> m_SkyVertex; ///< Colección de vértices reservada para el Skybox (actualmente comentada).

  /**
   * @brief Colección de índices que definen los triángulos de la geometría.
   */
  std::vector<unsigned int> m_index;

  int m_numVertex; ///< Cantidad total de vértices registrados en la malla.

  int m_numIndex;  ///< Cantidad total de índices registrados en la malla.
};