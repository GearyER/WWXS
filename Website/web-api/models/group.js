const group = (sequelize, DataTypes) => {
  const Group = sequelize.define('group', {
    name: {
      type: DataTypes.STRING,
      unique: true
    }
  });

  Group.associate = (models) => {
    Group.hasMany(models.User);
    Group.hasMany(models.Buoy);
  };

  Group.findByName = async (name) => Group.findOne({
    where: { name }
  });

  // Instance methods must be created with `Model.prototype.function`
  // Using an arrow function also does not allow you to reference `this`
  Group.prototype.addUserByPk = async function addUserFunc(uid) {
    sequelize.models.user.findByPk(uid)
      .then((user) => this.addUser(user))
      .catch((err) => console.error(err));
  };

  Group.prototype.removeUserByPk = async function removeUserFunc(uid) {
    sequelize.models.user.findByPk(uid)
      .then((user) => this.removeUser(user))
      .catch((err) => console.error(err));
  };

  Group.prototype.addBuoyByPk = async function addBuoyFunc(bid) {
    sequelize.models.buoy.findByPk(bid)
      .then((buoy) => this.addBuoy(buoy))
      .catch((err) => console.error(err));
  };

  Group.prototype.removeBuoyByPk = async function removeBuoyFunc(bid) {
    sequelize.models.buoy.findByPk(bid)
      .then((buoy) => this.removeBuoy(buoy))
      .catch((err) => console.error(err));
  };

  Group.prototype.isUserInGroup = async function userGroupFunc(uid) {
    return new Promise((resolve) => {
      this.getUsers().then((users) => {
        let found = false;
        users.forEach((user) => {
          if (user.dataValues.id === parseInt(uid, 10)) found = true;
        });

        resolve(found);
      });
    });
  };

  Group.prototype.isBuoyInGroup = async function buoyGroupFunc(bid) {
    return new Promise((resolve) => {
      this.getBuoys().then((buoys) => {
        let found = false;
        buoys.forEach((buoy) => {
          if (buoy.dataValues.id === parseInt(bid,10)) found = true;
        });

        resolve(found);
      });
    });
  };

  return Group;
};

module.exports = group;
