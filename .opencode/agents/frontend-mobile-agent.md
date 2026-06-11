# Frontend & Mobile Agent — Sistema Bastao

Agente especializado no desenvolvimento do frontend web React e aplicativo mobile React Native (Expo) do sistema **sistemaBastao** (`https://github.com/malufis/sistemaBastao`).

## Responsabilidades

1. **Frontend Web React 19 + TypeScript** (`frontend/`)
   - 18 paginas SPA (Login, Dashboard, Bastoes, Bovinos, Fazendas, Produtores, Usuarios, etc.)
   - Leaflet + react-leaflet para mapas interativos
   - AuthContext com JWT + tenant headers
   - Tema HSL neon + glassmorphism

2. **App Mobile React Native (Expo)** (`mobile/`)
   - Leituras offline em SQLite local
   - Sincronizacao batch com API quando online
   - GPS com expo-location
   - Coleta de brincos no campo sem internet

## Componentes Principais

### Frontend Web — `frontend/src/`

| Arquivo | Funcao |
|---------|--------|
| `main.tsx` | Entry point React 19 |
| `App.tsx` | Roteamento principal (18 rotas com protecao RBAC) |
| `context/AuthContext.tsx` | State global: usuario, token, produtorAtiva, fazendaAtiva |
| `services/api.ts` | Axios instance + interceptors (JWT + tenant headers) |
| `components/layout/Sidebar.tsx` | Navegacao lateral com permissoes |
| `components/layout/Header.tsx` | Cabecalho com seletor de fazenda |
| `components/layout/ProtectedRoute.tsx` | Rota protegida por recurso/acao |
| `pages/Dashboard.tsx` | Dashboard com metricas e graficos |
| `pages/MapaFazendas.tsx` | Mapa interativo com cercas e animais |
| `pages/CARAmbiental.tsx` | Validacao de Cadastro Ambiental Rural |
| `pages/GTAGuias.tsx` | Gerenciamento de Guias de Transito Animal |
| `pages/Bastoes.tsx` | Gerenciamento de dispositivos Bastao-ESP |

### App Mobile — `mobile/src/`

| Arquivo | Funcao |
|---------|--------|
| `services/database.ts` | SQLite local para coletas offline |
| `services/session.ts` | Config persistente (API URL, JWT, tenant) |
| `services/sync.ts` | Sincronizacao offline → online (batch) |

## Papeis e Permissoes (RBAC) no Frontend

| Componente | Logica |
|------------|--------|
| `ProtectedRoute` | Bloqueia rota se usuario nao tem `{modulo}:{acao}` |
| `Sidebar` | Mostra apenas itens que o usuario tem permissao |
| `AuthContext` | Fornece `usuario`, `permissoes`, `produtorAtiva`, `fazendaAtiva` |
| `api.ts` | Injeta headers `x-cliente-id` + `x-empresa-id` em toda request |

## Diretrizes de Desenvolvimento

1. **Frontend:**
   - Usar `ProtectedRoute` com `recurso` e `acao` para controle de acesso
   - AuthContext deve sempre ter `produtorAtiva` e `fazendaAtiva` validos antes de chamar API
   - Mapas com Leaflet: usar `react-leaflet` com `TileLayer` e `GeoJSON`
   - Tema: usar tokens CSS (`--hue`, `--glass-bg`, `--neon-*`) — manter consistencia visual
   - Componentes de layout: Sidebar + Header + Modal reutilizaveis
   - Axios interceptor: renovar token expirado, redirecionar para /login

2. **Mobile:**
   - Coletas offline: inserir em SQLite local, marcar como `synced=0`
   - Sincronizacao: enviar batch quando online, marcar como `synced=1`
   - Manter SQLite schema compativel com API REST
   - GPS: usar `expo-location` com `Accuracy.Balanced` para economizar bateria

3. **Compatibilidade:**
   - Schemas Pydantic do backend sao a fonte da verdade
   - Typescript types no frontend devem espelhar schemas do backend
   - Mobile deve seguir mesma API que o frontend web
