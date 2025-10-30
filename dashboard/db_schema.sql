-- Schema proposto para integração com Banco de Dados (Postgres / MySQL)
-- Tabelas: setores, vagas, veiculos, eventos

CREATE TABLE setores (
  id SERIAL PRIMARY KEY,
  codigo VARCHAR(4) UNIQUE NOT NULL,
  descricao TEXT,
  criado_em TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE vagas (
  id SERIAL PRIMARY KEY,
  setor_id INTEGER REFERENCES setores(id),
  numero INTEGER NOT NULL,
  estado VARCHAR(32) DEFAULT 'DISPONIVEL',
  atualizado_em TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE veiculos (
  id SERIAL PRIMARY KEY,
  identificador VARCHAR(128) UNIQUE NOT NULL,
  placa VARCHAR(32),
  modelo VARCHAR(128),
  criado_em TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE eventos (
  id SERIAL PRIMARY KEY,
  veiculo_id INTEGER REFERENCES veiculos(id),
  tipo VARCHAR(64),
  descricao TEXT,
  payload JSONB,
  criado_em TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Índices úteis
CREATE INDEX idx_vagas_setor ON vagas(setor_id);
CREATE INDEX idx_eventos_tipo ON eventos(tipo);
