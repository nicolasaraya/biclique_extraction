import sys

def leer_archivo(nombre_archivo):
    try:
        with open(nombre_archivo, 'r') as archivo:
            contenido = archivo.readlines()
    except FileNotFoundError:
        print(f"Error: El archivo '{nombre_archivo}' no se encuentra.")
        return None
    return contenido

def procesar_bicliques(contenido):
    adyacencias = {}
    i = 0

    while i < len(contenido):
        if contenido[i].startswith("S:"):
            s_nodos = contenido[i][2:].strip().split()
            i += 1
            if i < len(contenido) and contenido[i].startswith("C:"):
                c_nodos = contenido[i][2:].strip().split()

                for s in s_nodos:
                    if s not in adyacencias:
                        adyacencias[s] = []
                    adyacencias[s].extend(c_nodos)
                    adyacencias[s] = sorted(set(adyacencias[s]))
            else:
                print("Advertencia: Se esperaba una línea que empezara con 'C:' después de una línea que empieza con 'S:'. Línea saltada.")
        else:
            print("Advertencia: Línea saltada ya que no empieza con 'S:' o 'C:'.")
        i += 1

    adyacencias_ordenadas = dict(sorted(adyacencias.items(), key=lambda x: int(x[0])))
    return adyacencias_ordenadas

def guardar_adyacencias(nombre_archivo, adyacencias):
    nombre_salida = nombre_archivo.replace(".txt", "_toAdj.txt")

    try:
        with open(nombre_salida, 'w') as archivo:
            aristas = sum(len(adyacentes) for adyacentes in adyacencias.values())
            archivo.write(f"Cantidad de aristas: {aristas}\n")
            for nodo, adyacentes in adyacencias.items():
                archivo.write(f"{nodo}: {' '.join(adyacentes)}\n")
        print(f"Listas de adyacencia guardadas en '{nombre_salida}'")
    except IOError:
        print(f"Error: No se pudo guardar el archivo '{nombre_salida}'.")

def main():
    if len(sys.argv) != 2:
        print("Uso: python3 BicliqueToAdy.py file.txt")
        return

    nombre_archivo = sys.argv[1]
    contenido = leer_archivo(nombre_archivo)

    if contenido:
        adyacencias = procesar_bicliques(contenido)

        if adyacencias:
            guardar_adyacencias(nombre_archivo, adyacencias)

if __name__ == "__main__":
    main()
