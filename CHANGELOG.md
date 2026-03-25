# Changelog

Toutes les modifications sont documentées selon [Semantic Versioning](https://semver.org/).

## [2.0.0-dev] - 2026-03-24
### Modified
- Refonte du système de factures et d'offres. Données plus exhaustives.
### Added
- Le comparateur d'offre permet de comparer avec précisément les offres.
- Taxes et charges dans les offres
- Grilles tarifaires selon la puissance du compteur (électricité)

## [1.1.0-dev] - 2026-03-24
### Added
- Comptes administrateurs
- Offres
- Comparateur d'offres

## [1.0.0-dev] - 2026-03-24
### Modified
- Refonte du système de factures
- Refonte de l'interface
### Added
- Comptes utilisateurs
- Contrats
- Serialization des données (comptes utilisateurs) au format JSON

## [0.3.0-dev] - 2026-03-20
### Added
- Système de création de factures avec champs dynamiques

## [0.2.1-dev] - 2026-03-19
### Added
- Fichier styles .qss chargeable par **QWidget** avec le **StylesHelper** 
### Modify
- Réécriture de parties de codes

## [0.2.0-dev] - 2026-03-18
### Added
- Options "Quitter" et "Nouvelle Facture" dans la QMenuBar
- Icons/Images dans le fichier ressources.qrc
- Onglets "Factures", "Estimations", "Comparateur" dans un QTabWidget de la QMainWindow
- Un QMdiArea pour l'onglet "Factures"

## [0.1.0-dev] - 2026-03-17
### Added
- Page de Connexion (LoginDialog) avec email/identifiant et mot de passe
- Validation simple des champs