/**
 * @file Triangle.hlsl
 * @brief Shaders de prueba para renderizado 2D básico (sin transformaciones de cámara).
 * 
 * Utilizado para validar el pipeline gráfico dibujando geometría simple directamente 
 * en el espacio de pantalla (Normalized Device Coordinates).
 */

/**
 * @struct VSInput
 * @brief Estructura de entrada para el Vertex Shader.
 */
struct VSInput
{
    /** @brief Posición del vértice en 3D (X, Y, Z). Semántica: POSITION. */
    float3 position : POSITION;
    
    /** @brief Color RGBA del vértice. Semántica: COLOR. */
    float4 color : COLOR;
};
/**
 * @struct PSInput
 * @brief Estructura de intercambio entre el Vertex Shader y el Pixel Shader.
 */
struct PSInput
{
    /** @brief Posición final del vértice. Semántica del sistema: SV_POSITION. */
    float4 position : SV_POSITION;
    
    /** @brief Color a interpolar sobre la superficie del polígono. Semántica: COLOR. */
    float4 color : COLOR;
};
/**
 * @brief Punto de entrada del Vertex Shader.
 * 
 * Convierte el vector de posición de 3D a 4D agregando un valor W de 1.0f.
 * A diferencia de Cube.hlsl, no aplica multiplicaciones matriciales, por lo que 
 * los vértices de entrada deben estar ya en el rango [-1, 1] de la pantalla.
 * 
 * @param input Datos del vértice leídos desde el Vertex Buffer.
 * @return PSInput Datos procesados enviados a la etapa de rasterización.
 */
PSInput VSMain(VSInput input)
{
    PSInput output;
    
    // Paso directo de coordenadas locales a la pantalla (NDC) sin proyección.
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    
    return output;
}
/**
 * @brief Punto de entrada del Pixel Shader.
 * 
 * @param input Datos interpolados provenientes del rasterizador.
 * @return float4 Color final del píxel renderizado en el Render Target. Semántica: SV_TARGET.
 */
float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}