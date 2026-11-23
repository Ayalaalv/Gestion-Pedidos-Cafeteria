#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>
using namespace std;

string aMinusculas(string s) {
    for (char& c : s) c = (char)tolower((unsigned char)c);
    return s;
}

string recortar(const string& s) {
    size_t ini = s.find_first_not_of(" \t\r\n");
    if (ini == string::npos) return "";
    size_t fin = s.find_last_not_of(" \t\r\n");
    return s.substr(ini, fin - ini + 1);
}

string leerTexto(const string& mensaje) {
    string linea;
    while (true) {
        cout << mensaje;
        if (!getline(cin, linea)) exit(0);
        linea = recortar(linea);
        if (!linea.empty()) return linea;
        cout << "  ! El valor no puede estar vacío.\n";
    }
}

int leerEntero(const string& mensaje, int minimo, int maximo) {
    while (true) {
        string linea = leerTexto(mensaje);
        try {
            size_t pos;
            int valor = stoi(linea, &pos);
            if (pos == linea.size() && valor >= minimo && valor <= maximo) return valor;
        }
        catch (...) {}
        cout << "  ! Ingresa un número entero entre " << minimo << " y " << maximo << ".\n";
    }
}

double leerDecimal(const string& mensaje, double minimo) {
    while (true) {
        string linea = leerTexto(mensaje);
        for (char& c : linea) if (c == ',') c = '.';
        try {
            size_t pos;
            double valor = stod(linea, &pos);
            if (pos == linea.size() && valor >= minimo) return valor;
        }
        catch (...) {}
        cout << "  ! Ingresa un número mayor o igual a " << minimo << ".\n";
    }
}

bool leerSiNo(const string& mensaje) {
    while (true) {
        string r = aMinusculas(leerTexto(mensaje + " (s/n): "));
        if (r == "s" || r == "si" || r == "sí") return true;
        if (r == "n" || r == "no") return false;
        cout << "  ! Responde 's' o 'n'.\n";
    }
}

void pausar() {
    cout << "\nPresiona Enter para continuar...";
    string tmp;
    if (!getline(cin, tmp)) exit(0);
}

string horaActual() {
    time_t t = time(nullptr);
    tm info;
#ifdef _WIN32
    localtime_s(&info, &t);
#else
    localtime_r(&t, &info);
#endif
    char buf[20];
    strftime(buf, sizeof(buf), "%H:%M:%S", &info);
    return buf;
}

string dinero(double valor) {
    ostringstream os;
    os << fixed << setprecision(2) << valor;
    return os.str();
}

string columna(const string& s, size_t ancho) {
    string res;
    size_t visibles = 0;
    for (size_t i = 0; i < s.size() && visibles < ancho;) {
        unsigned char c = s[i];
        size_t len = (c < 0x80) ? 1 : ((c >> 5) == 6) ? 2 : ((c >> 4) == 14) ? 3 : 4;
        res += s.substr(i, len);
        i += len;
        visibles++;
    }
    res.append(ancho - visibles, ' ');
    return res;
}

string derecha(const string& s, size_t ancho) {
    return s.size() >= ancho ? s : string(ancho - s.size(), ' ') + s;
}

void titulo(const string& texto) {
    cout << "\n==================================================\n  " << texto
        << "\n==================================================\n";
}

struct Producto {
    int codigo = 0;
    string nombre;
    string categoria;
    double precio = 0.0;
    bool disponible = true;
};

void encabezadoProductos() {
    cout << columna("Código", 8) << columna("Nombre", 24) << columna("Categoría", 16)
        << derecha("Precio", 10) << "  Disponible\n"
        << string(70, '-') << "\n";
}

void mostrarProducto(const Producto& p) {
    cout << columna(to_string(p.codigo), 8) << columna(p.nombre, 24) << columna(p.categoria, 16)
        << derecha(dinero(p.precio), 10) << "  " << (p.disponible ? "Sí" : "No") << "\n";
}

void fichaProducto(const Producto& p) {
    cout << "  Código      : " << p.codigo << "\n"
        << "  Nombre      : " << p.nombre << "\n"
        << "  Categoría   : " << p.categoria << "\n"
        << "  Precio      : " << dinero(p.precio) << "\n"
        << "  Disponible  : " << (p.disponible ? "Sí" : "No") << "\n";
}

class ArregloProductos {
    Producto* datos;
    int tamano;
    int capacidad;

    void redimensionar() {
        capacidad *= 2;
        Producto* nuevo = new Producto[capacidad];
        for (int i = 0; i < tamano; i++) nuevo[i] = datos[i];
        delete[] datos;
        datos = nuevo;
    }

public:
    explicit ArregloProductos(int capacidadInicial = 4)
        : datos(new Producto[capacidadInicial]), tamano(0), capacidad(capacidadInicial) {
    }
    ~ArregloProductos() { delete[] datos; }
    ArregloProductos(const ArregloProductos&) = delete;
    ArregloProductos& operator=(const ArregloProductos&) = delete;

    void agregar(const Producto& p) {
        if (tamano == capacidad) redimensionar();
        datos[tamano++] = p;
    }
    int getTamano() const { return tamano; }
    int getCapacidad() const { return capacidad; }
    bool estaVacio() const { return tamano == 0; }
    Producto& obtener(int i) { return datos[i]; }
    void intercambiar(int i, int j) {
        Producto tmp = datos[i];
        datos[i] = datos[j];
        datos[j] = tmp;
    }
};

int main() {

    return 0;
}
enum Criterio { SIN_ORDEN = 0, POR_CODIGO = 1, POR_NOMBRE = 2, POR_PRECIO = 3 };

string nombreCriterio(Criterio c) {
    switch (c) {
    case POR_CODIGO: return "código";
    case POR_NOMBRE: return "nombre";
    case POR_PRECIO: return "precio";
    default:         return "sin ordenar";
    }
}

int comparar(const Producto& a, const Producto& b, Criterio c) {
    switch (c) {
    case POR_CODIGO: return (a.codigo > b.codigo) - (a.codigo < b.codigo);
    case POR_NOMBRE: {
        int r = aMinusculas(a.nombre).compare(aMinusculas(b.nombre));
        return (r > 0) - (r < 0);
    }
    case POR_PRECIO: return (a.precio > b.precio) - (a.precio < b.precio);
    default:         return 0;
    }
}

int particionar(ArregloProductos& arr, int bajo, int alto, Criterio c) {
    Producto pivote = arr.obtener(alto);
    int i = bajo - 1;
    for (int j = bajo; j < alto; j++) {
        if (comparar(arr.obtener(j), pivote, c) <= 0) {
            i++;
            arr.intercambiar(i, j);
        }
    }
    arr.intercambiar(i + 1, alto);
    return i + 1;
}

void quickSort(ArregloProductos& arr, int bajo, int alto, Criterio c) {
    if (bajo < alto) {
        int p = particionar(arr, bajo, alto, c);
        quickSort(arr, bajo, p - 1, c);
        quickSort(arr, p + 1, alto, c);
    }
}

int busquedaSecuencial(ArregloProductos& arr, const Producto& clave, Criterio c,
    int desde, int& comparaciones) {
    for (int i = desde; i < arr.getTamano(); i++) {
        comparaciones++;
        if (comparar(arr.obtener(i), clave, c) == 0) return i;
    }
    return -1;
}

int busquedaBinaria(ArregloProductos& arr, const Producto& clave, Criterio c, int& comparaciones) {
    int izq = 0, der = arr.getTamano() - 1;
    while (izq <= der) {
        int medio = izq + (der - izq) / 2;
        comparaciones++;
        int r = comparar(arr.obtener(medio), clave, c);
        if (r == 0) return medio;
        if (r < 0) izq = medio + 1;
        else       der = medio - 1;
    }
    return -1;
}
struct Pedido {
    int numero = 0;
    string cliente;
    int codigoProducto = 0;
    string nombreProducto;
    int cantidad = 0;
    double precioUnitario = 0.0;
    double total = 0.0;
    string estado = "Pendiente";
};

struct NodoPedido {
    Pedido dato;
    NodoPedido* siguiente;
    explicit NodoPedido(const Pedido& p) : dato(p), siguiente(nullptr) {}
};

void encabezadoPedidos() {
    cout << columna("N°", 6) << columna("Cliente", 18) << columna("Producto", 20)
        << derecha("Cant", 5) << derecha("P.Unit", 10) << derecha("Total", 11) << "  Estado\n"
        << string(82, '-') << "\n";
}

void mostrarPedido(const Pedido& p) {
    cout << columna(to_string(p.numero), 6) << columna(p.cliente, 18) << columna(p.nombreProducto, 20)
        << derecha(to_string(p.cantidad), 5) << derecha(dinero(p.precioUnitario), 10)
        << derecha(dinero(p.total), 11) << "  " << p.estado << "\n";
}

void fichaPedido(const Pedido& p) {
    cout << "  Número          : " << p.numero << "\n"
        << "  Cliente         : " << p.cliente << "\n"
        << "  Producto        : " << p.nombreProducto << " (cód. " << p.codigoProducto << ")\n"
        << "  Cantidad        : " << p.cantidad << "\n"
        << "  Precio unitario : " << dinero(p.precioUnitario) << "\n"
        << "  Total           : " << dinero(p.total) << "\n"
        << "  Estado          : " << p.estado << "\n";
}

class ColaPedidos {
    NodoPedido* frente;
    NodoPedido* final;
    int cantidad;

public:
    ColaPedidos() : frente(nullptr), final(nullptr), cantidad(0) {}
    ~ColaPedidos() {
        Pedido tmp;
        while (desencolar(tmp)) {}
    }
    ColaPedidos(const ColaPedidos&) = delete;
    ColaPedidos& operator=(const ColaPedidos&) = delete;

    void encolar(const Pedido& p) {
        NodoPedido* nuevo = new NodoPedido(p);
        if (final == nullptr) frente = nuevo;
        else final->siguiente = nuevo;
        final = nuevo;
        cantidad++;
    }

    bool desencolar(Pedido& salida) {
        if (frente == nullptr) return false;
        NodoPedido* aux = frente;
        salida = aux->dato;
        frente = frente->siguiente;
        if (frente == nullptr) final = nullptr;
        delete aux;
        cantidad--;
        return true;
    }

    Pedido* verFrente() { return frente ? &frente->dato : nullptr; }
    bool estaVacia() const { return frente == nullptr; }
    int getCantidad() const { return cantidad; }

    void mostrar() const {
        for (NodoPedido* n = frente; n != nullptr; n = n->siguiente) mostrarPedido(n->dato);
    }

    Pedido* buscar(int numero) {
        for (NodoPedido* n = frente; n != nullptr; n = n->siguiente)
            if (n->dato.numero == numero) return &n->dato;
        return nullptr;
    }
};