/**
 * @file Cube.hlsl
 * @brief Código HLSL que contiene el Vertex Shader y Pixel Shader básicos para renderizar geometría (como un cubo) con colores por vértice.
 */

/**
 * @brief Búfer de constantes (Constant Buffer) mapeado al registro b0.
 * 
 * Se actualiza desde la CPU con los datos de transformación para el frame actual.
 */
cbuffer TransformBuffer : register(b0)
{
    /** @brief Matriz combinada de Mundo, Vista y Proyección (World-View-Projection). */
    float4x4 worldViewProjection;
};
/**
 * @struct VSInput
 * @brief Estructura que define los datos de entrada por cada vértice (Vertex Input Layout).
 */
struct VSInput
{
    /** @brief Posición local del vértice en espacio 3D. Semántica: POSITION. */
    float3 position : POSITION;
    
    /** @brief Color RGBA asignado al vértice. Semántica: COLOR. */
    float4 color : COLOR;
};
/**
 * @struct PSInput
 * @brief Estructura que define la salida del Vertex Shader y la entrada al Pixel Shader.
 */
struct PSInput
{
    /** @brief Posición transformada al espacio de clip/pantalla. Semántica del sistema: SV_POSITION. */
    float4 position : SV_POSITION;
    
    /** @brief Color interpolado por el rasterizador para el fragmento actual. Semántica: COLOR. */
    float4 color : COLOR;
};
/**
 * @brief Punto de entrada del Vertex Shader (Shader de Vértices).
 * 
 * Transforma la posición local de cada vértice al espacio de la cámara/pantalla
 * utilizando la matriz de Mundo-Vista-Proyección. Transmite el color sin alteraciones.
 * 
 * @param input Datos de entrada del vértice actual provenientes del Input Assembler.
 * @return PSInput Datos transformados listos para la etapa de rasterización.
 */
PSInput VSMain(VSInput input)
{
    PSInput output;
    
    // Multiplica el vector de posición 4D (x, y, z, 1.0) por la matriz WVP.
    output.position = mul(
        float4(input.position, 1.0f),
        worldViewProjection
    );
    
    output.color = input.color;
    
    return output;
}
/**
 * @brief Punto de entrada del Pixel Shader (Shader de Píxeles/Fragmentos).
 * 
 * Se ejecuta por cada píxel de la geometría rasterizada en pantalla.
 * 
 * @param input Datos del fragmento interpolados por el rasterizador.
 * @return float4 El color final del píxel en formato RGBA que se escribirá en el Render Target. Semántica: SV_TARGET.
 */
float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}