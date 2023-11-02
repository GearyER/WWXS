const express = require('express');

const router = express.Router();

/* Get all location histories for a specific buoy */
router.get('/buoy/:buoyId', async (req, res) => {
  const query = {
    where: { buoyId: req.params.buoyId },
    include: [
      { model: req.context.models.Location },
      { model: req.context.models.Buoy }
    ]
  };

  req.context.models.History.findAll(query)
    .then((histories) => res.status(200).json(histories))
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Get a specific location history entry */
router.get('/:historyId', async (req, res) => {
  req.context.models.History.findByPk(req.params.historyId, {
    include: [
      { model: req.context.models.Location },
      { model: req.context.models.Buoy }
    ]
  })
    .then((history) => {
      if (history) {
        return res.status(200).json(history);
      } else {
        return res.status(404).json({ message: 'Not Found' });
      }
    })
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Create a new location history entry */
router.post('/', async (req, res) => {
  req.context.models.History.create(req.body)
    .then((history) => res.status(201).json(history))
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Delete a location history entry */
router.delete('/:historyId', async (req, res) => {
  req.context.models.History.destroy({
    where: { id: req.params.historyId }
  })
    .then((n) => {
      if (n > 0) {
        return res.status(200).json({ message: 'OK' });
      } else {
        return res.status(404).json({ message: 'Not Found' });
      }
    })
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

module.exports = router;
