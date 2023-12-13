const manager = (sequelize, DataTypes) => {
  const Manager = sequelize.define('manager', {
    name: {
      type: DataTypes.STRING,
      unique: true
    }
  });

  Manager.associate = (models) => {
    Manager.belongsTo(models.User);
    Manager.belongsTo(models.Buoy);
  };

  return Manager;
};

module.exports = manager;

