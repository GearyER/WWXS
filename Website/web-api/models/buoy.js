const buoy = (sequelize, DataTypes) => {
  const Buoy = sequelize.define('buoy', {
    name: {
      type: DataTypes.STRING,
      unique: true,
    },
    mac: {
      type: DataTypes.STRING,
      unique: true,
    },
    pubKey: {
      type: DataTypes.STRING,
      unique: true,
    },
    lastRetrieval: {
      type: DataTypes.DATE,
    },
    version: {
      type: DataTypes.STRING,
    }
  });

  Buoy.associate = (models) => {
    // Each buoy belongs to one group
    Buoy.belongsTo(models.Group);

    // Each buoy has one owner, a user
    Buoy.belongsTo(models.User);

    // Each buoy can have multiple location histories
    Buoy.hasMany(models.LocationHistory);
  };

  // Before creating a buoy, validate that the user set as the owner has the 'owner' role
  // Buoy.beforeCreate(async (buoy, options) => {
  //   const user = await User.findByPk(buoy.userId); 
  //   if (user && user.role === 'owner') {
  //     buoy.ownerId = buoy.userId;
  //   } else {
  //     throw new Error('The provided user is not an owner.');
  //   }
  // });

  return Buoy;
};

module.exports = buoy;
