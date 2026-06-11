---
name: frontend-react
description: Diretrizes para desenvolvimento do frontend React SPA e app mobile Expo do sistemaBastao, incluindo RBAC, mapas Leaflet, offline SQLite e sincronizacao.
---

# Frontend React & Mobile Skill — Sistema Bastao

Esta skill rege o desenvolvimento do frontend web React SPA e aplicativo mobile React Native (Expo) da plataforma **sistemaBastao**.

---

## 1. Frontend Web (React 19 + TypeScript + Vite)

### 1.1. Estrutura do Projeto
```
frontend/
├── index.html
├── vite.config.ts          # Proxy /api → api:8000
├── package.json            # React 19, Vite 8, TS 6
├── src/
│   ├── main.tsx            # Entry point
│   ├── App.tsx             # Router (18 rotas)
│   ├── App.css / index.css # Tema HSL + glassmorphism
│   ├── pages/              # 17 paginas
│   │   ├── Dashboard.tsx
│   │   ├── Bastoes.tsx
│   │   ├── Bovinos.tsx
│   │   ├── Fazendas.tsx
│   │   ├── Produtores.tsx
│   │   ├── CARAmbiental.tsx
│   │   ├── GTAGuias.tsx
│   │   ├── MapaFazendas.tsx
│   │   ├── ImportarCadastro.tsx
│   │   ├── Estoque.tsx
│   │   ├── SimCards.tsx
│   │   ├── PapeisPermissoes.tsx
│   │   ├── Auditoria.tsx
│   │   ├── Login.tsx
│   │   ├── Usuarios.tsx
│   │   ├── UsuarioForm.tsx
│   │   └── UsuarioPermissoes.tsx
│   ├── components/layout/
│   │   ├── Sidebar.tsx      # Navegacao com permissoes RBAC
│   │   ├── Header.tsx       # Cabecalho + seletor de fazenda
│   │   ├── ProtectedRoute.tsx # Rota protegida RBAC
│   │   └── Modal.tsx        # Modal reutilizavel
│   ├── components/mapa/     # Componentes Leaflet
│   ├── context/
│   │   └── AuthContext.tsx  # State global (usuario, token, fazenda)
│   ├── services/
│   │   └── api.ts           # Axios instance + interceptors
│   └── assets/
```

### 1.2. Autenticacao e RBAC

```typescript
// AuthContext provê:
interface AuthContextType {
  usuario: UsuarioInfo | null;
  token: string | null;
  produtorAtiva: number | null;   // header x-cliente-id
  fazendaAtiva: number | null;    // header x-empresa-id
  permissoes: Record<string, string[]>;
  login(email: string, senha: string): Promise<void>;
  logout(): void;
  temPermissao(modulo: string, acao: string): boolean;
}
```

```tsx
// Rota protegida
<ProtectedRoute recurso="animais" acao="ler">
  <Animais />
</ProtectedRoute>

// Sidebar condicional
{temPermissao('bastoes', 'ler') && <SidebarItem icon={Cpu} label="Bastões" to="/bastoes" />}
```

### 1.3. Axios Interceptor (api.ts)
```typescript
api.interceptors.request.use((config) => {
  config.headers.Authorization = `Bearer ${token}`;
  config.headers['x-cliente-id'] = produtorAtiva;
  config.headers['x-empresa-id'] = fazendaAtiva;
  return config;
});

api.interceptors.response.use(
  (response) => response,
  (error) => {
    if (error.response?.status === 401) redirecionarLogin();
    return Promise.reject(error);
  }
);
```

### 1.4. Mapas Leaflet
- `react-leaflet` com `TileLayer` (OpenStreetMap ou similar)
- `MapContainer`, `TileLayer`, `GeoJSON`, `Marker`, `Popup`
- GeoJSON carregado da API `/fazendas/{id}/geometria`
- `leaflet-draw` para edicao de cercas

### 1.5. Tema CSS
```css
:root {
  --hue: 160;
  --neon-green: hsl(var(--hue), 80%, 50%);
  --glass-bg: hsla(var(--hue), 60%, 20%, 0.3);
  --glass-border: hsla(var(--hue), 60%, 50%, 0.2);
  --bg-gradient: linear-gradient(135deg, #0a0f1a, #0d1a0f);
}
```

## 2. App Mobile (React Native + Expo)

### 2.1. Estrutura
```
mobile/
├── App.tsx              # Entry point: leituras offline + sync
├── src/services/
│   ├── database.ts      # SQLite local
│   ├── session.ts       # Config persistente
│   └── sync.ts          # Sincronizacao
```

### 2.2. SQLite Offline (database.ts)
```typescript
// Schema local
CREATE TABLE coletas_offline (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  brinco_id TEXT NOT NULL,
  latitude REAL,
  longitude REAL,
  timestamp TEXT NOT NULL,
  synced INTEGER DEFAULT 0
);

// Insercao offline
function salvarColetaOffline(brinco_id, latitude, longitude) {
  db.runAsync(
    'INSERT INTO coletas_offline (brinco_id, latitude, longitude, timestamp) VALUES (?, ?, ?, ?)',
    brinco_id, latitude, longitude, new Date().toISOString()
  );
}
```

### 2.3. Sincronizacao Batch (sync.ts)
```typescript
async function sincronizar() {
  const pendentes = await db.getAllAsync(
    'SELECT * FROM coletas_offline WHERE synced = 0'
  );
  for (const coleta of pendentes) {
    try {
      await api.post('/coletas', coleta);
      await db.runAsync('UPDATE coletas_offline SET synced = 1 WHERE id = ?', coleta.id);
    } catch (error) {
      console.error('Falha ao sincronizar coleta', coleta.id, error);
    }
  }
}
```

## 3. Diretrizes de Desenvolvimento

1. **RBAC consistente:** Frontend e Mobile devem usar mesmos recursos/acoes definidos no backend
2. **Estado global:** AuthContext como unica fonte de verdade para usuario, token e tenant
3. **Offline-first:** Mobile deve funcionar sem internet; sincronizar quando online
4. **Performance:** Evitar re-renders desnecessarios; usar React.memo em componentes pesados
5. **Mapas:** Nao carregar GeoJSON inteiro de uma vez; usar bounding box ou paginacao
6. **Seguranca:** Token JWT armazenado em sessionStorage (web) ou SecureStore (mobile)
7. **Responsividade:** Layout adaptavel para desktop e tablet
