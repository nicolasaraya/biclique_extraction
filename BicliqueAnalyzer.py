import sys
import numpy as np
import matplotlib.pyplot as plt

def count_edges_distribution(filepath):
    """
    Lee un archivo (omitiendo la primera línea) y devuelve:
    - Una lista `bicliques` con datos de cada biclique (aristas, tamaño de S, tamaño de C).
    - Una lista `edges_list` con solo el número de aristas de cada biclique, ordenado de menor a mayor.
    """
    bicliques = []  # Lista para almacenar (aristas, tamaño de S, tamaño de C)
    edges_list = []  # Lista para almacenar solo el número de aristas de cada biclique
    S_sizes_list = []
    C_sizes_list = []
    Coef_list = []

    try:
        with open(filepath, 'r') as file:
            lines = file.readlines()[1:]  # Omitir la primera línea
            for i in range(0, len(lines), 2):  # Procesar líneas de dos en dos
                line_s = lines[i].strip()
                line_c = lines[i + 1].strip()

                if line_s.startswith("S:") and line_c.startswith("C:"):
                    # Obtener los números después de ':' y convertirlos en listas
                    s_numbers = line_s.split(":")[1].strip().split()
                    c_numbers = line_c.split(":")[1].strip().split()

                    # Calcular el número de aristas
                    s_size = len(s_numbers)
                    c_size = len(c_numbers)
                    edges = s_size * c_size

                    # Agregar datos completos a `bicliques`
                    bicliques.append((edges, s_size, c_size))

                    # Agregar solo el número de aristas a `edges_list`
                    edges_list.append(edges)
                    S_sizes_list.append(s_size)
                    C_sizes_list.append(c_size)
                    Coef_list.append(edges / (s_size + c_size))

            # Ordenar la lista `edges_list` antes de devolverla
            edges_list.sort()
            S_sizes_list.sort()
            C_sizes_list.sort()
            Coef_list.sort()
        return bicliques, edges_list, S_sizes_list, C_sizes_list, Coef_list
    except FileNotFoundError:
        print(f"Error: El archivo {filepath} no se encontró.")
        return None, None, None, None, None
    except IndexError:
        print(f"Error: Formato incorrecto en el archivo {filepath}. Asegúrate de que las líneas S: y C: estén correctamente pareadas.")
        return None, None, None, None, None

def analyze_bicliques(bicliques):
    """
    Imprime un resumen general de los bicliques analizados.
    """
    if not bicliques:
        return

    # Calcular estadísticas generales
    total_bicliques = len(bicliques)
    total_edges = sum(b[0] for b in bicliques)
    average_edges = total_edges / total_bicliques if total_bicliques > 0 else 0

    # Encontrar el biclique de menor y mayor tamaño
    min_biclique = min(bicliques, key=lambda x: x[0])  # Menor cantidad de aristas
    max_biclique = max(bicliques, key=lambda x: x[0])  # Mayor cantidad de aristas

    # Imprimir el análisis
    print(f"  Bicliques: {total_bicliques}")
    print(f"  Aristas totales: {total_edges}")
    print(f"  Aristas promedio: {average_edges:.2f}")
    print(f"  Biclique de menor tamaño:")
    print(f"    Aristas: {min_biclique[0]}, Tamaño de S: {min_biclique[1]}, Tamaño de C: {min_biclique[2]}")
    print(f"  Biclique de mayor tamaño:")
    print(f"    Aristas: {max_biclique[0]}, Tamaño de S: {max_biclique[1]}, Tamaño de C: {max_biclique[2]}")

def filter_by_percentile(elementList, lower_percentile=10, upper_percentile=90):
    """
    Filtra `elementList` para incluir solo los elementos entre los percentiles dados.
    """
    if not elementList:
        return []

    # Calcular los valores de los percentiles
    lower_bound = np.percentile(elementList, lower_percentile)
    upper_bound = np.percentile(elementList, upper_percentile)

    # Filtrar los elementos que están dentro del rango
    return [i for i in elementList if lower_bound <= i <= upper_bound]


def subdivide_common_edges(edges_lists, num_subdivisions=10):
    """
    Divide múltiples `edges_list` según divisiones comunes basadas en el rango mínimo y máximo.
    """
    # Obtener el rango común (mínimo y máximo entre todas las listas)
    all_edges = [edge for edges_list in edges_lists for edge in edges_list]
    min_val = min(all_edges)
    max_val = max(all_edges)
    range_size = (max_val - min_val) / num_subdivisions  # Tamaño de cada subdivisión

    subdivisions_list = []
    for edges_list in edges_lists:
        subdivisions = {i: [] for i in range(1, num_subdivisions + 1)}
        for edge in edges_list:
            # Calcular a qué subdivisión pertenece
            group = int((edge - min_val) // range_size) + 1
            if group > num_subdivisions:  # Asegurar que el máximo caiga en la última subdivisión
                group = num_subdivisions
            subdivisions[group].append(edge)
        subdivisions_list.append(subdivisions)

    # Generar etiquetas de intervalos comunes
    interval_labels = []
    for i in range(1, num_subdivisions + 1):
        lower_bound = min_val + range_size * (i - 1)
        upper_bound = min_val + range_size * i
        interval_labels.append(f"{int(lower_bound)} - {int(upper_bound)}")

    return subdivisions_list, interval_labels

def plot_comparison_line_chart(subdivisions_list, interval_labels, filenames, log_vertical=False, log_horizontal=False):
    """
    Genera un gráfico de líneas comparativo para múltiples datasets.
    Opcionalmente usa escalas logarítmicas para los ejes vertical y horizontal.
    """
    # Crear el gráfico de comparación
    plt.figure(figsize=(12, 6))

    for i, (subdivisions, filename) in enumerate(zip(subdivisions_list, filenames)):
        # Preparar datos para cada dataset
        division_counts = [len(values) for values in subdivisions.values()]  # Cantidad de elementos por subdivisión
        plt.plot(interval_labels, division_counts, marker='o', linestyle='-', label=f'{filename}', linewidth=2)

    plt.xlabel('Intervalos')
    plt.ylabel('Cantidad de Bicliques')
    plt.title('Comparación de Datasets (Divisiones Comunes)')
    plt.xticks(rotation=45, ha='right')  # Rotar las etiquetas del eje X para mejor visualización

    # Aplicar escala logarítmica si es necesario
    if log_vertical:
        plt.yscale('log')
    if log_horizontal:
        plt.xscale('log')

    plt.legend()  # Añadir leyenda para diferenciar los datasets
    plt.tight_layout()

    # Guardar el gráfico como imagen
    output_filename = "comparacion_grafico_linea_divisiones_comunes.png"
    plt.savefig(output_filename)
    plt.close()  # Cerrar el gráfico para liberar memoria
    print(f"Gráfico comparativo guardado como: {output_filename}")
  
def plot_comparison_line_chart_S(subdivisions_list, interval_labels, filenames, log_vertical=False, log_horizontal=False):
    """
    Genera un gráfico de líneas comparativo para múltiples datasets.
    Opcionalmente usa escalas logarítmicas para los ejes vertical y horizontal.
    """
    # Crear el gráfico de comparación
    plt.figure(figsize=(12, 6))

    for i, (subdivisions, filename) in enumerate(zip(subdivisions_list, filenames)):
        # Preparar datos para cada dataset
        division_counts = [len(values) for values in subdivisions.values()]  # Cantidad de elementos por subdivisión
        plt.plot(interval_labels, division_counts, marker='o', linestyle='-', label=f'{filename}', linewidth=2)

    plt.xlabel('Intervalos de S')
    plt.ylabel('Cantidad de Bicliques')
    plt.title('Comparación de Datasets (Divisiones Comunes)')
    plt.xticks(rotation=45, ha='right')  # Rotar las etiquetas del eje X para mejor visualización

    # Aplicar escala logarítmica si es necesario
    if log_vertical:
        plt.yscale('log')
    if log_horizontal:
        plt.xscale('log')

    plt.legend()  # Añadir leyenda para diferenciar los datasets
    plt.tight_layout()

    # Guardar el gráfico como imagen
    output_filename = "comparacion_grafico_linea_divisiones_comunes_S.png"
    plt.savefig(output_filename)
    plt.close()  # Cerrar el gráfico para liberar memoria
    print(f"Gráfico comparativo guardado como: {output_filename}")

def plot_comparison_line_chart_C(subdivisions_list, interval_labels, filenames, log_vertical=False, log_horizontal=False):
    """
    Genera un gráfico de líneas comparativo para múltiples datasets.
    Opcionalmente usa escalas logarítmicas para los ejes vertical y horizontal.
    """
    # Crear el gráfico de comparación
    plt.figure(figsize=(12, 6))

    for i, (subdivisions, filename) in enumerate(zip(subdivisions_list, filenames)):
        # Preparar datos para cada dataset
        division_counts = [len(values) for values in subdivisions.values()]  # Cantidad de elementos por subdivisión
        plt.plot(interval_labels, division_counts, marker='o', linestyle='-', label=f'{filename}', linewidth=2)

    plt.xlabel('Intervalos de C')
    plt.ylabel('Cantidad de Bicliques')
    plt.title('Comparación de Datasets (Divisiones Comunes)')
    plt.xticks(rotation=45, ha='right')  # Rotar las etiquetas del eje X para mejor visualización

    # Aplicar escala logarítmica si es necesario
    if log_vertical:
        plt.yscale('log')
    if log_horizontal:
        plt.xscale('log')

    plt.legend()  # Añadir leyenda para diferenciar los datasets
    plt.tight_layout()

    # Guardar el gráfico como imagen
    output_filename = "comparacion_grafico_linea_divisiones_comunes_C.png"
    plt.savefig(output_filename)
    plt.close()  # Cerrar el gráfico para liberar memoria
    print(f"Gráfico comparativo guardado como: {output_filename}")

def plot_comparison_line_chart_Coef(subdivisions_list, interval_labels, filenames, log_vertical=False, log_horizontal=False):
    """
    Genera un gráfico de líneas comparativo para múltiples datasets.
    Opcionalmente usa escalas logarítmicas para los ejes vertical y horizontal.
    """
    # Crear el gráfico de comparación
    plt.figure(figsize=(12, 6))

    for i, (subdivisions, filename) in enumerate(zip(subdivisions_list, filenames)):
        # Preparar datos para cada dataset
        division_counts = [len(values) for values in subdivisions.values()]  # Cantidad de elementos por subdivisión
        plt.plot(interval_labels, division_counts, marker='o', linestyle='-', label=f'{filename}', linewidth=2)

    plt.xlabel('Intervalos de Coef')
    plt.ylabel('Cantidad de Bicliques')
    plt.title('Comparación de Datasets (Divisiones Comunes)')
    plt.xticks(rotation=45, ha='right')  # Rotar las etiquetas del eje X para mejor visualización

    # Aplicar escala logarítmica si es necesario
    if log_vertical:
        plt.yscale('log')
    if log_horizontal:
        plt.xscale('log')

    plt.legend()  # Añadir leyenda para diferenciar los datasets
    plt.tight_layout()

    # Guardar el gráfico como imagen
    output_filename = "comparacion_grafico_linea_divisiones_comunes_Coef.png"
    plt.savefig(output_filename)
    plt.close()  # Cerrar el gráfico para liberar memoria
    print(f"Gráfico comparativo guardado como: {output_filename}")


def plot_line_chart(subdivisions, interval_labels, filename, log_vertical=False, log_horizontal=False):
    """
    Genera y guarda un gráfico de línea basado en las subdivisiones calculadas para un dataset.
    Opcionalmente usa escalas logarítmicas para los ejes vertical y horizontal.
    """
    # Preparar datos para el gráfico
    division_counts = [len(values) for values in subdivisions.values()]  # Cantidad de elementos por subdivisión

    # Crear el gráfico de línea
    plt.figure(figsize=(12, 6))
    plt.plot(interval_labels, division_counts, marker='o', linestyle='-', color='blue')
    plt.xlabel('Intervalos')
    plt.ylabel('Cantidad de Bicliques')
    plt.title(f'Gráfico de Línea para {filename}')
    plt.xticks(rotation=45, ha='right')  # Rotar las etiquetas del eje X para mejor visualización

    # Aplicar escala logarítmica si es necesario
    if log_vertical:
        plt.yscale('log')
    if log_horizontal:
        plt.xscale('log')

    plt.tight_layout()

    # Guardar el gráfico como imagen
    output_filename = f"{filename}_grafico_linea.png"
    plt.savefig(output_filename)
    plt.close()  # Cerrar el gráfico para liberar memoria
    print(f"Gráfico de línea guardado como: {output_filename}")

def plot_line_chart_S(subdivisions, interval_labels, filename, log_vertical=False, log_horizontal=False):
    """
    Genera y guarda un gráfico de línea basado en las subdivisiones calculadas para un dataset.
    Opcionalmente usa escalas logarítmicas para los ejes vertical y horizontal.
    """
    # Preparar datos para el gráfico
    division_counts = [len(values) for values in subdivisions.values()]  # Cantidad de elementos por subdivisión

    # Crear el gráfico de línea
    plt.figure(figsize=(12, 6))
    plt.plot(interval_labels, division_counts, marker='o', linestyle='-', color='blue')
    plt.xlabel('Intervalos de S')
    plt.ylabel('Cantidad de Bicliques')
    plt.title(f'Gráfico de Línea para {filename}')
    plt.xticks(rotation=45, ha='right')  # Rotar las etiquetas del eje X para mejor visualización

    # Aplicar escala logarítmica si es necesario
    if log_vertical:
        plt.yscale('log')
    if log_horizontal:
        plt.xscale('log')

    plt.tight_layout()

    # Guardar el gráfico como imagen
    output_filename = f"{filename}_grafico_linea_S.png"
    plt.savefig(output_filename)
    plt.close()  # Cerrar el gráfico para liberar memoria
    print(f"Gráfico de línea guardado como: {output_filename}")

def plot_line_chart_C(subdivisions, interval_labels, filename, log_vertical=False, log_horizontal=False):
    """
    Genera y guarda un gráfico de línea basado en las subdivisiones calculadas para un dataset.
    Opcionalmente usa escalas logarítmicas para los ejes vertical y horizontal.
    """
    # Preparar datos para el gráfico
    division_counts = [len(values) for values in subdivisions.values()]  # Cantidad de elementos por subdivisión

    # Crear el gráfico de línea
    plt.figure(figsize=(12, 6))
    plt.plot(interval_labels, division_counts, marker='o', linestyle='-', color='blue')
    plt.xlabel('Intervalos de C')
    plt.ylabel('Cantidad de Bicliques')
    plt.title(f'Gráfico de Línea para {filename}')
    plt.xticks(rotation=45, ha='right')  # Rotar las etiquetas del eje X para mejor visualización

    # Aplicar escala logarítmica si es necesario
    if log_vertical:
        plt.yscale('log')
    if log_horizontal:
        plt.xscale('log')

    plt.tight_layout()

    # Guardar el gráfico como imagen
    output_filename = f"{filename}_grafico_linea_C.png"
    plt.savefig(output_filename)
    plt.close()  # Cerrar el gráfico para liberar memoria
    print(f"Gráfico de línea guardado como: {output_filename}")

def plot_line_chart_Coef(subdivisions, interval_labels, filename, log_vertical=False, log_horizontal=False):
    """
    Genera y guarda un gráfico de línea basado en las subdivisiones calculadas para un dataset.
    Opcionalmente usa escalas logarítmicas para los ejes vertical y horizontal.
    """
    # Preparar datos para el gráfico
    division_counts = [len(values) for values in subdivisions.values()]  # Cantidad de elementos por subdivisión

    # Crear el gráfico de línea
    plt.figure(figsize=(12, 6))
    plt.plot(interval_labels, division_counts, marker='o', linestyle='-', color='blue')
    plt.xlabel('Intervalos de Coef')
    plt.ylabel('Cantidad de Bicliques')
    plt.title(f'Gráfico de Línea para {filename}')
    plt.xticks(rotation=45, ha='right')  # Rotar las etiquetas del eje X para mejor visualización

    # Aplicar escala logarítmica si es necesario
    if log_vertical:
        plt.yscale('log')
    if log_horizontal:
        plt.xscale('log')

    plt.tight_layout()

    # Guardar el gráfico como imagen
    output_filename = f"{filename}_grafico_linea_Coef.png"
    plt.savefig(output_filename)
    plt.close()  # Cerrar el gráfico para liberar memoria
    print(f"Gráfico de línea guardado como: {output_filename}")

def main():
    """Función principal para procesar múltiples archivos y generar gráficos."""
    if len(sys.argv) < 3:
        print("Uso: python3 script.py archivo1 archivo2 ...")
        sys.exit(1)

    # Procesar múltiples archivos desde los argumentos
    files = sys.argv[1:]  # Lista de archivos desde los argumentos
    edges_lists = []
    bicliques_list = []
    S_sizes_lists = []
    C_sizes_lists = []
    Coef_lists = []

    lower_percentile = 20
    upper_percentile = 70

    for filepath in files:
        print(f"\nProcesando archivo: {filepath}")
        bicliques, edges_list, S_sizes_list, C_sizes_list, Coef_list = count_edges_distribution(filepath)
        if edges_list:
            bicliques_list.append(bicliques)
            # Filtrar las aristas por percentiles (0% a 90% por defecto)
            filtered_edges = filter_by_percentile(edges_list, lower_percentile, upper_percentile)
            edges_lists.append(filtered_edges)
            # Subdividir los elementos para este archivo
            subdivisions, interval_labels = subdivide_common_edges([filtered_edges])
            # Generar el gráfico individual para este archivo
            plot_line_chart(subdivisions[0], interval_labels, filepath)

            filtered_S_sizes = filter_by_percentile(S_sizes_list, lower_percentile, upper_percentile)
            S_sizes_lists.append(filtered_S_sizes)
            subdivisions, interval_labels = subdivide_common_edges([filtered_S_sizes])
            plot_line_chart_S(subdivisions[0], interval_labels, filepath)

            filtered_C_sizes = filter_by_percentile(C_sizes_list, lower_percentile, upper_percentile)
            C_sizes_lists.append(filtered_C_sizes)
            subdivisions, interval_labels = subdivide_common_edges([filtered_C_sizes])
            plot_line_chart_C(subdivisions[0], interval_labels, filepath)

            filtered_Coef = filter_by_percentile(Coef_list, lower_percentile, upper_percentile)
            Coef_lists.append(filtered_Coef)
            subdivisions, interval_labels = subdivide_common_edges([filtered_Coef])
            plot_line_chart_Coef(subdivisions[0], interval_labels, filepath)

            # Analizar e imprimir detalles de los bicliques
            analyze_bicliques(bicliques)

    # Subdividir todos los elementos según divisiones comunes para el gráfico combinado
    subdivisions_list, interval_labels = subdivide_common_edges(edges_lists, 30)

    # Generar el gráfico combinado
    plot_comparison_line_chart(subdivisions_list, interval_labels, files)

    subdivisions_list, interval_labels = subdivide_common_edges(S_sizes_lists, 30)
    plot_comparison_line_chart_S(subdivisions_list, interval_labels, files)
    
    subdivisions_list, interval_labels = subdivide_common_edges(C_sizes_lists, 30)
    plot_comparison_line_chart_C(subdivisions_list, interval_labels, files)

    subdivisions_list, interval_labels = subdivide_common_edges(Coef_lists, 10)
    plot_comparison_line_chart_Coef(subdivisions_list, interval_labels, files)


if __name__ == "__main__":
    main()
