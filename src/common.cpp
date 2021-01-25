QSettings Settings() {
  static const QSettings::Format format = QSettings::registerFormat(
      QStringLiteral("json"), &KtUtils::SettingsExtra::jsonReadFunc,
      &KtUtils::SettingsExtra::jsonWriteFunc, Qt::CaseInsensitive);
  static const QFileInfo fileInfo(qApp->applicationDirPath() +
                                  "/config/config.json");
  return QSettings(fileInfo.absoluteFilePath(), format);
}
