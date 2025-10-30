// Template de adaptador de banco (substitua por sua lib favorita)
// Exemplos: pg (node-postgres), sequelize, knex, mysql2, typeorm...
module.exports = {
  async saveEvent(event){
    // event: objeto com dados
    // TODO: implementar persistência real em DB
    console.log("DB.saveEvent (template) chamado:", event);
    return true;
  }
}
