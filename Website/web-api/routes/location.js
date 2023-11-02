const express = require('express');
const router = express.Router();
const sequelize = require('sequelize');
const Op = sequelize.Op;

// How many degrees apart coordinates must be before they'll be considered distinct.
// This is kind of janky, but MUCH simpler than evenly tiling a sphere.
// Potential TODO: devise a better scheme for avoiding overlapping locations,
// and reusing locations, than this.
const spacing = 0.001; // at the equator, 0.001 degrees ~= 111 meters.

/* Get all locations */
router.get('/', async (req, res) => {
  /* begin a SELECT query */
  req.context.models.Location.findAll()
    /* Return the JSON array */
    .then((locations) => res.status(200).json(locations))
    /* Handle errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Check for locations nearby a certain location
 * Used with a GET request to /location/nearby?lat=x&lon=y
 * latitude and longitude are simply decimal values, i.e, "12.5454"
 */
router.get('/near/', async (req, res) => {
  if(!req.query.lat || !req.query.lon) {
    return res.status(400).json({ message: 'Latitude and longitude must be specified' });
  }

  const query = {};
  const lat = parseFloat(req.query.lat);
  const lon = parseFloat(req.query.lon);

  if(lat == NaN || lon == NaN) {
    return res.status(400).json({ message: 'Latitude and longitude must be numeric' });
  }

  query.where = {
    latitude: {
      [Op.between]: [lat - spacing, lat + spacing]
    },

    longitude: {
      [Op.between]: [lon - spacing, lon + spacing]
    }
  };

  req.context.models.Location.findAll(query)
    /* Return the JSON array */
    .then((locations) => {
      if(locations.length == 0) {
        return res.status(204).json({ message: 'Not Found' });
      } else if(locations.length == 1) {
        return res.status(200).json(locations[0]);
      } else {
        return res.status(400).json({ message: 'Multiple candidate locations found' });
      }
    })
    /* Handle errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

// Get specific location by ID
router.get('/:locId', async(req, res) => {
  req.context.models.Location.findByPk(req.params.locId)
    .then((loc) => {
      if(loc) {
        return res.status(200).json(loc);
      } else {
        return res.status(404).json({ message: 'Not Found' });
      }
    })
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

// Create a new location
router.post('/', async(req, res) => {
  if(!req.body.latitude || !req.body.longitude) {
    return res.status(400).json({ message: 'Latitude and longitude must be specified' });
  }

  const query = {};
  const lat = req.body.latitude;
  const lon = req.body.longitude;

  query.where = {
    latitude: {
      [Op.between]: [lat - spacing, lat + spacing]
    },

    longitude: {
      [Op.between]: [lon - spacing, lon + spacing]
    }
  };

  // Add buoy at a location
  router.post('/:locId/addBuoy/:buoyId', async (req, res) => {
    const locId = req.params.locId;
    const buoyId = req.params.buoyId;

    const newHistory = await req.context.models.History.create({
      buoyId: buoyId,
      locationId: locId
    });
    if (newHistory) {
      return res.status(201).json(newHistory);
    } else {
      return res.status(400).json({ message: 'Failed to log buoy at location' });
    }
  });


  req.context.models.Location.findAll(query)
    /* Return the JSON array */
    .then((locations) => {
      if(locations.length >= 1) {
        return res.status(400).json({ message: 'Location already exists' });
      }
    })
    /* Handle errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad request' });
    });


  req.context.models.Location.create(req.body)
    .then((location) => res.status(201).json(location))
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});


/* Remove a location */
router.delete('/:groupId', async (req, res) => {
  /* Query the db and delete the group */
  req.context.models.Location.destroy({
    where: { id: req.params.groupId }
  })
  .then((n) => {
    /* Check the number of affected rows */
    if (n > 0) {
      return res.status(200).json({ message: 'OK' });
    } else {
      /* There was no location with that id, not found */
      return res.status(404).json({ message: 'Not Found' });
    }
  })
  /* Handle errors */
  .catch((err) => {
    console.error(err);
    return res.status(400).json({ message: 'Bad Request' });
  })
});

module.exports = router;
