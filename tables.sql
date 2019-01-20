-
-- Structure de la table `relever`
--

CREATE TABLE `relever` (
  `id` int(11) NOT NULL,
  `temperature` int(11) NOT NULL,
  `humidité` tinyint(2) NOT NULL,
  `date` timestamp NULL DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--

--
-- Index pour la table `relever`
--
ALTER TABLE `relever`
  ADD PRIMARY KEY (`id`);


--
-- AUTO_INCREMENT pour la table `relever`
--
ALTER TABLE `relever`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;